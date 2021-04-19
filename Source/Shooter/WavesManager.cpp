#include "WavesManager.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
#include "ShooterCharacter.h"

AWavesManager::AWavesManager()
{
}

void AWavesManager::LaunchNextWave() {
	currentWave++;

	for (int i = 0; i < currentWave; ++i) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn enemy"));
		listEnemiesAlive.Emplace(target->GetWorld()->SpawnActor<AEnemy>(BP_Enemy, portalLocation, FRotator(0, 0, 0)));
	}
}