// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class AShooterCharacter;
UCLASS()
class SHOOTER_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int hp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int damage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	AShooterCharacter* targetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float attackDelay;

	float lastAttack = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool takeDamage(int value);
	bool CanAttack();
	void SetTarget(AShooterCharacter* target);
	int GetDamage() { return damage; }

	void SetDamageScale(int value) { damage *= value; }
	void SetHPScale(int value) { hp *= value; }
	void SetSizeScale(int value) { SetActorScale3D(FVector(value)); }
};
