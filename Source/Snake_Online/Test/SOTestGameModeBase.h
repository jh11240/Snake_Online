// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SOTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API ASOTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	ASOTestGameModeBase();
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void SpawnFood();
	void SpawnFood(FVector SpawnLoc);
private:
	FVector GetRandomLocation();

	FVector MinBound= FVector(-150.000000, -250.000000, 140.000000);
	FVector MaxBound = FVector(990.000000, 530.000000, 140.000000);

	// 타이머 핸들 변수 선언
	FTimerHandle FoodSpawnTimerHandle;

	float curSec = 0.f;
	float spawnSec = 3.f;
};
