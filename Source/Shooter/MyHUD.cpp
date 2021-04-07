// Fill out your copyright notice in the Description page of Project Settings.
#include "MyHUD.h"

AMyHUD::AMyHUD() {
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AMyHUD::BeginPlay() {
	Super::BeginPlay();

	if (GunWidgetClass) {
		GunWidget = CreateWidget<UGunWidget>(GetWorld(), GunWidgetClass);
		if (GunWidget) {
			GunWidget->AddToViewport();
		}
	}
}

void AMyHUD::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
}

void AMyHUD::DrawHUD() {
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition((Center.X),
		(Center.Y));

	// draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}

void AMyHUD::UpdateMagazineWidget(int32 bulletsAmount, int32 capacity) {
	if (GunWidget) {
		GunWidget->UpdateMagazineWidget(bulletsAmount, capacity);
	}
}

void AMyHUD::toggleReloading() {
	if (GunWidget) {
		GunWidget->toggleReloading();
	}
}