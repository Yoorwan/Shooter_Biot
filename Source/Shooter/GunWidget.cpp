// Fill out your copyright notice in the Description page of Project Settings.
#include "GunWidget.h"

UGunWidget::UGunWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
}

void UGunWidget::NativeConstruct() {
	Super::NativeConstruct();
	TXTReloading->SetText(FText::FromString("Reloading..."));
	TXTReloading->SetVisibility(ESlateVisibility::Hidden);
}

void UGunWidget::UpdateMagazineWidget(int32 bulletsAmount, int32 capacity) {
	if(TXTMagazine->Visibility == ESlateVisibility::Hidden)
		TXTMagazine->SetVisibility(ESlateVisibility::Visible);
	
	TXTMagazine->SetText(FText::FromString(FString::FromInt(bulletsAmount) + " / " + FString::FromInt(capacity)));
}

void UGunWidget::toggleReloading() {
	if (TXTReloading->Visibility == ESlateVisibility::Hidden) {
		TXTReloading->SetVisibility(ESlateVisibility::Visible);
	}
	else if (TXTReloading->Visibility == ESlateVisibility::Visible) {
		TXTReloading->SetVisibility(ESlateVisibility::Hidden);
	}	
}