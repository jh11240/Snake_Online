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
	virtual void Logout(AController* Exiting) override;
	virtual void Tick(float DeltaTime) override;
	void SpawnFood();
	void SpawnFood(FVector SpawnLoc);
private:
	FVector GetRandomLocation();

	FVector MinBound= FVector(-460.000000, -600.000000, 140.000000);
	FVector MaxBound = FVector(1180.000000, 1150.000000, 140.000000);

	// 타이머 핸들 변수 선언
	FTimerHandle FoodSpawnTimerHandle;

	float curSec = 0.f;
	float spawnSec = 3.f;
};
