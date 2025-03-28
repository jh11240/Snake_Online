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
private:
	void SpawnFood();
	FVector GetRandomLocation();

	FVector MinBound= FVector(-2360.0, -2360.0, 140.0);
	FVector MaxBound = FVector(2310., 2300.,141.);

	// 타이머 핸들 변수 선언
	FTimerHandle FoodSpawnTimerHandle;
};
