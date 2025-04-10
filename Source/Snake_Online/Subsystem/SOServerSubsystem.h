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
	USOServerSubsystem();
public:
	void AddLobbyPlayerToQueue(APlayerController* Player);
	void SetPlayerSetting(uint32 PlayerID, FPlayerSettings playerSetting);
	void TryMatchPlayers();
	void AddSnakePlayerToQueue(APlayerController* Player);
public:
	//Properties
	void SetClientID(uint32 ID) { ClientUniqueID = ID; }
	int32 GetClientID() { return ClientUniqueID; }
	int32 GetStartDirIdx() { return startDir++; }
	int32 GetNewClientID() { return ++LastClientUniqueID; }
	int32 GetPlace() { return gamePrize--; }
	void ClientExit(AController* Exiting);
	void ExitGameServer();
	FPlayerSettings GetPlayerSetting(uint32 id);

private:
	TArray<APlayerController*> PlayerQueue;

#pragma region 서버 관리용
	bool isMatchEntered = false;
	//한 룸당 매칭 인원
	int32 MatchPlayerNum = 2;
	//클라이언트 번호와 유저세팅 맵
	UPROPERTY()
	TMap<int32, FPlayerSettings> playerSettingMap;
	//마지막으로 접속한 클라이언트 번호
	int32 LastClientUniqueID =-1;
	//시작 방향 -> 
	// 클라이언트 번호로 할수있지만 현재 클라이언트 번호는 onRep_Pawn에서 처리
	// 복제가 늦게 되면 시작 시 무적시간처리가 귀찮아져서 
	// 그냥 서버onpossess에서 미리 해당 방향으로움직이게함 클라는 움직이는 거 복제
	int32 startDir = 0;

	//등수
	int32 gamePrize=2;
#pragma endregion

#pragma region 클라이언트용
	
	uint32 ClientUniqueID;
#pragma endregion
};
