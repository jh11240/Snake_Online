	// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SOServerSubsystem.generated.h"

USTRUCT()
struct FPlayerSettings
{
	GENERATED_BODY()

	UPROPERTY()
	FText PlayerName;

	UPROPERTY()
	uint32 PlayerMaterialIdx;
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
	void SetPlayerSetting(uint32 PlayerID, FPlayerSettings playerSetting);
	
	void TryMatchPlayers();
public:
	//Properties
	void SetClientID(uint32 ID) { ClientUniqueID = ID; }
	uint32 GetClientID() { return ClientUniqueID; }
	uint32 GetStartDirIdx() { return startDir++; }
	uint32 GetNewClientID() { return ++LastClientUniqueID; }
	FPlayerSettings GetPlayerSetting(uint32 id);
private:
	TArray<APlayerController*> PlayerQueue;

	//서버 관리용
	UPROPERTY()
	TMap<uint32, FPlayerSettings> playerSettingMap;
	uint32 LastClientUniqueID =-1;
	uint32 startDir = 0;

	//클라이언트용
	uint32 ClientUniqueID;
};
