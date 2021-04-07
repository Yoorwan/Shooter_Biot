#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Pistol.generated.h"

UCLASS()
class SHOOTER_API APistol : public AWeapon
{
	GENERATED_BODY()
	
public:
	void Shoot();
};
