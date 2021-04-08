// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

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
	AActor* targetActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector target;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool takeDamage(int value);
};
