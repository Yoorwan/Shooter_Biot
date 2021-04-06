#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UActorComponent;

UCLASS(Abstract, Blueprintable)
class SHOOTER_API UWeapon : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float rateOfFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float lastShot = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float magazineCapacity;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float reloadTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float damagePerBullet;

public:
	UWeapon();
	
	void Shoot();
};
