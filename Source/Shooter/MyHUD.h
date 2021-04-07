// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Components/WidgetComponent.h"
#include "GunWidget.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateMagazineWidget(int32 bulletsAmount, int32 capacity);

	UFUNCTION()
	void toggleReloading();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UUserWidget> GunWidgetClass;
	
	UGunWidget* GunWidget;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
};
