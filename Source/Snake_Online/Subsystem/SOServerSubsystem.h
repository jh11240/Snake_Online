	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SOServerSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSettings
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FText PlayerName;

	UPROPERTY(BlueprintReadWrite)
	UMaterialInterface* PlayerMaterial;
};

/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API USOServerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	void AddPlayerToQueue(APlayerController* Player);
	void AddPlayerToQueue(APlayerController* Player,FPlayerSettings* playerSetting);
	
	void TryMatchPlayers();

private:
	TArray<APlayerController*> PlayerQueue;
	TMap<APlayerController*, FPlayerSettings> playerSettingMap;
};
