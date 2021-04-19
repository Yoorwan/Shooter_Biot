#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WavesManager.generated.h"

class AEnemy;
class AShooterCharacter;

UCLASS()
class SHOOTER_API AWavesManager : public AActor
{
	GENERATED_BODY()
private:
	int currentWave = 0;

	const FVector portalLocation = FVector(-1950, -762, 680);

	TArray<AEnemy*> listEnemiesAlive;
	TArray<AEnemy*> listEnemiesDead;

public:
	AWavesManager();

	void LaunchNextWave();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Waves)
	AShooterCharacter* target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Waves)
	TSubclassOf<AEnemy> BP_Enemy;
};
