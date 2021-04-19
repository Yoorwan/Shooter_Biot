// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "ShooterCharacter.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float directionX = targetActor->GetActorLocation().X - GetActorLocation().X;
	float directionY = targetActor->GetActorLocation().Y - GetActorLocation().Y;
	FVector direction(directionX, directionY, 0);
	direction.Normalize();

	SetActorLocation(GetActorLocation() + direction * 200 * DeltaTime);
}

bool AEnemy::takeDamage(int value) {
	hp -= value;
	return hp <= 0;
}

bool AEnemy::CanAttack() {
	if (lastAttack + attackDelay < GetGameTimeSinceCreation()) {
		lastAttack = GetGameTimeSinceCreation();
		return true;
	}
	return false;
}