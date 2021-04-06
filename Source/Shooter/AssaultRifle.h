// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
class UWeapon;
UCLASS()
class SHOOTER_API UAssaultRifle : public UWeapon
{
	GENERATED_BODY()
public:

	void Shoot() override;
};
