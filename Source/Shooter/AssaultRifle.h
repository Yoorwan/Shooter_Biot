#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AssaultRifle.generated.h"

UCLASS()
class SHOOTER_API AAssaultRifle : public AWeapon
{
	GENERATED_BODY()

public:
	void Shoot();
};
