// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterCharacter.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Pistol.h"
#include "AssaultRifle.h"
#include "Containers/Array.h"
#include "MyHUD.h"
#include "WavesManager.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "Engine/World.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include <Runtime/Engine/Public/DrawDebugHelpers.h>

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AShooterCharacter

AShooterCharacter::AShooterCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(Mesh1P);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void AShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	FP_MuzzleLocation->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}

	AWeapon* tempWeapon = GetWorld()->SpawnActor<APistol>(pistolWeapon, FVector(0, 0, 0), FRotator(0, 0, 0));
	tempWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	tempWeapon->SetActorRelativeLocation(FVector(0,-4,-2));
	weaponsList.Emplace(tempWeapon);

	tempWeapon = GetWorld()->SpawnActor<AAssaultRifle>(assaultRifleWeapon, FVector(0, 0, 0), FRotator(0, 0, 0));
	tempWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	tempWeapon->SetActorRelativeLocation(FVector(0, -4, -2));
	weaponsList.Emplace(tempWeapon);

	weaponsList[1]->SetActorHiddenInGame(true);
	
	MyHUD = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	UpdateHUD();
}

void AShooterCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	CheckPlayerCollisions();
	OnFire();
}

void AShooterCharacter::UpdateHUD() {
	if (MyHUD) {
		MyHUD->UpdateMagazineWidget(weaponsList[currentWeapon]->currentMagazineCapacity, weaponsList[currentWeapon]->magazineCapacity);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AShooterCharacter::StopFire);

	// Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::Reload);

	PlayerInputComponent->BindAction("SwitchWeapon1", IE_Pressed, this, &AShooterCharacter::SwitchToPistol);
	PlayerInputComponent->BindAction("SwitchWeapon2", IE_Pressed, this, &AShooterCharacter::SwitchToAR);

	PlayerInputComponent->BindAction("PrevWeapon", IE_Pressed, this, &AShooterCharacter::PrevWeapon);
	PlayerInputComponent->BindAction("NextWeapon", IE_Pressed, this, &AShooterCharacter::NextWeapon);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AShooterCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
}

void AShooterCharacter::Shoot() {
	UWorld* const World = GetWorld();
	if (World != nullptr) {
		if (weaponsList[currentWeapon]->lastShot + weaponsList[currentWeapon]->rateOfFire < GetGameTimeSinceCreation()) {
			
			weaponsList[currentWeapon]->lastShot = GetGameTimeSinceCreation();
			
			FRotator rotation = GetControlRotation();
			FVector gunOffset(0, 0, 25);
			UGameplayStatics::SpawnEmitterAtLocation(World, PS_GunShot, weaponsList[currentWeapon]->GetActorLocation() + weaponsList[currentWeapon]->GetActorForwardVector(), rotation, true);
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());

			FHitResult hitResult;
			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this);

			FVector start = FirstPersonCameraComponent->GetComponentLocation() + GetActorForwardVector();
			FVector end = start + 5000.0 * FirstPersonCameraComponent->GetForwardVector();

			if (GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECollisionChannel::ECC_PhysicsBody, Params)) {
				UGameplayStatics::SpawnEmitterAtLocation(World, PS_GunImpact, hitResult.ImpactPoint, rotation, true);
				
				if (hitResult.GetActor()->IsA(AEnemy::StaticClass())) {
					if (Cast<AEnemy>(hitResult.GetActor())) {
						if (Cast<AEnemy>(hitResult.GetActor())->takeDamage(weaponsList[currentWeapon]->damagePerBullet)) {
							wavesManager->DeleteEnemy(Cast<AEnemy>(hitResult.GetActor()));
							hitResult.GetActor()->Destroy();
						}
					}
				}
			}
			--weaponsList[currentWeapon]->currentMagazineCapacity;
			UpdateHUD();
		}
		
	}
}

void AShooterCharacter::SwitchToPistol()
{
	if (!isReloading) {
		weaponsList[currentWeapon]->SetActorHiddenInGame(true);
		currentWeapon = 0;
		weaponsList[currentWeapon]->SetActorHiddenInGame(false);
		UpdateHUD();
	}	
}

void AShooterCharacter::SwitchToAR()
{
	if (!isReloading) {
		weaponsList[currentWeapon]->SetActorHiddenInGame(true);
		currentWeapon = 1;
		weaponsList[currentWeapon]->SetActorHiddenInGame(false);
		UpdateHUD();
	}
}

void AShooterCharacter::PrevWeapon() {
	if (!isReloading) {
		if (currentWeapon != 0) {
			--currentWeapon;
		}
		else {
			currentWeapon = weaponsList.Num() - 1;
		}
		UpdateHUD();
	}
}

void AShooterCharacter::NextWeapon() {
	if (!isReloading) {
		if (currentWeapon != weaponsList.Num() - 1) {
			++currentWeapon;
		}
		else {
			currentWeapon = 0;
		}
		UpdateHUD();
	}
}

void AShooterCharacter::Reload()
{
	if (!isReloading) {
		isReloading = true;

		if (MyHUD) {
			MyHUD->ToggleReloading();
		}

		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(
			UnusedHandle, this, &AShooterCharacter::OnReload, weaponsList[currentWeapon]->reloadTime, false);
	}
}

void AShooterCharacter::OnReload()
{
	weaponsList[currentWeapon]->currentMagazineCapacity = weaponsList[currentWeapon]->magazineCapacity;
	UpdateHUD();
	if (MyHUD) {
		MyHUD->ToggleReloading();
	}
	isReloading = false;
}

void AShooterCharacter::StartFire()
{
	isFiring = true;
}

void AShooterCharacter::OnFire()
{
	if (isFiring && !isReloading) {
		if (weaponsList[currentWeapon]->currentMagazineCapacity > 0) {
			if (weaponsList[currentWeapon]->isAuto) {
				Shoot();
			}
			else {
				if (!hasFired) {
					Shoot();
				}
			}
			hasFired = true;
		}
		else {
			Reload();
		}
	}
}

void AShooterCharacter::StopFire()
{
	hasFired = false;
	isFiring = false;
}

void AShooterCharacter::CheckPlayerCollisions() {
	TArray<AActor*> overlappingActors;
	GetOverlappingActors(overlappingActors, AEnemy::StaticClass());

	if (overlappingActors.Num() > 0) {
		for (AActor* enemyActor : overlappingActors)
		{
			AEnemy* enemy = Cast<AEnemy>(enemyActor);
			if (enemy->CanAttack()) {
				UE_LOG(LogTemp, Warning, TEXT("Health, %d"), currentHealth / playerHealth);
				if (takeDamage(enemy->GetDamage())) {
					if (MyHUD) {
						MyHUD->ToggleReloading();
					}
					UGameplayStatics::SetGamePaused(GetWorld(), true);
				}
			}
		}
	}
}

bool AShooterCharacter::takeDamage(int amount) {
	currentHealth -= amount;
	if (MyHUD) {
		MyHUD->UpdateHealthBar(currentHealth, playerHealth);
	}
	return currentHealth <= 0;
}

void AShooterCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AShooterCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AShooterCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void AShooterCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}

void AShooterCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AShooterCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShooterCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AShooterCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AShooterCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AShooterCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AShooterCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
