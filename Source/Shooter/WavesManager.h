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
	int currentWave = 1;

	const FVector portalLocation = FVector(-1950, -762, 500);

	float timer;
	float cooldown = 1;

	int enemiesSpawned = 0;

public:
	AWavesManager();

	void SpawnEnemy();
	void DeleteEnemy(AEnemy* enemyToDelete);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Waves)
	AShooterCharacter* target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Waves)
	TSubclassOf<AEnemy> BP_Enemy;

	TArray<AEnemy*> listEnemiesAlive;

protected:
	virtual void Tick(float DeltaTime);
};
