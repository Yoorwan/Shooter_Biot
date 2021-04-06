// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SHOOTER_API UWeapon : public UActorComponent
{
	GENERATED_BODY()
public:
	UWeapon();
	
	virtual void Shoot() PURE_VIRTUAL(UWeapon::Shoot);
};
