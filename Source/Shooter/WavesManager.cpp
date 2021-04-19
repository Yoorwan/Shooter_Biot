#include "WavesManager.h"
#include "Enemy.h"
#include "ShooterCharacter.h"

AWavesManager::AWavesManager(){
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;
}

void AWavesManager::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (enemiesSpawned < currentWave) {
		if (timer < GetGameTimeSinceCreation()) {
			SpawnEnemy();
			timer = GetGameTimeSinceCreation() + cooldown;
		}
	}
}

void AWavesManager::SpawnEnemy() {
	UWorld* world = target->GetWorld();
	if (world) {
		if (BP_Enemy) {
			AEnemy* tempEnemy = world->SpawnActor<AEnemy>(BP_Enemy, portalLocation, FRotator(0, 90, 0));
			tempEnemy->SetTarget(target);

			if (currentWave % 5 == 0) {
				if (enemiesSpawned == currentWave - 1) {
					tempEnemy->SetDamageScale(currentWave / 5 + 1);
					tempEnemy->SetHPScale(currentWave / 5 + 1);
					tempEnemy->SetSizeScale(2);
				}				
			}
			listEnemiesAlive.Emplace(tempEnemy);
			++enemiesSpawned;
		}
	}	
}

void AWavesManager::DeleteEnemy(AEnemy* enemyToDelete) {
	for (AEnemy* enemy : listEnemiesAlive) {
		if (enemyToDelete == enemy) {
			listEnemiesAlive.Remove(enemy);
		}
	}

	if (listEnemiesAlive.Num() == 0 && enemiesSpawned == currentWave) {
		++currentWave;
		enemiesSpawned = 0;
		SpawnEnemy();
	}
}