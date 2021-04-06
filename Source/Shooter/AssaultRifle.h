#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "AssaultRifle.generated.h"

UCLASS()
class SHOOTER_API UAssaultRifle : public UWeapon
{
	GENERATED_BODY()

public:
	void Shoot();
};
