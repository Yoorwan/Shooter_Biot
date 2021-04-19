// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "GunWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UGunWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UGunWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateMagazineWidget(int bulletsAmount, int capacity);

	void ToggleReloading();

	void UpdateHealthBar(int currentHealth, int maxHealth);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TXTMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* TXTReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* PBHealthBar;
};
