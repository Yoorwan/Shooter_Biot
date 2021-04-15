#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class AShooterCharacter;

UCLASS(Abstract, Blueprintable)
class SHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float rateOfFire;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	float lastShot = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float magazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Gameplay)
	float currentMagazineCapacity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float reloadTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float damagePerBullet;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool isAuto;
public:

public:
	AWeapon();
	
	void Shoot();
};
