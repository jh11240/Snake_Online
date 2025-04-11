// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SOServerSubsystem.h"
#include "Utils/NetworkUtils.h"
#include "Net/UnrealNetwork.h"
#include "Test/SOTestCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

USOServerSubsystem::USOServerSubsystem() : gamePrize(MatchPlayerNum)
{
}

void USOServerSubsystem::AddLobbyPlayerToQueue(APlayerController* Player)
{
    if (!Player) return;
    PlayerQueue.Add(Player);

    TryMatchPlayers();
}
void USOServerSubsystem::SetPlayerSetting(uint32 PlayerID, FPlayerSettings  playerSetting)
{
    UE_LOG(LogTemp, Warning, TEXT("SetPlayerSetting 서버에서 실행됨!"));
    playerSettingMap.Add(PlayerID, playerSetting );
       //playerSettingMap[PlayerID] = playerSetting;
}

void USOServerSubsystem::TryMatchPlayers()
{
        UE_LOG(LogTemp, Warning, TEXT("TryMatchPlayers 서버에서 실행됨!"));
        if (PlayerQueue.Num() < MatchPlayerNum)
            return;


        FString TravelCommand = SO::NetworkUtils::GetGameMapURL();
// UE_EDITOR
        PlayerQueue.Empty();
        isMatchEntered = true;
        GetWorld()->ServerTravel(TravelCommand);
        //Player1->ClientTravel(TravelCommand, TRAVEL_Absolute);
        //Player2->ClientTravel(TravelCommand, TRAVEL_Absolute);
}

void USOServerSubsystem::AddSnakePlayerToQueue(APlayerController* Player)
{

    if (!Player) return;
    PlayerQueue.Add(Player);
}

void USOServerSubsystem::ClientExit(AController* Exiting)
{
    APlayerController* ExitingPlayer = Cast<APlayerController>(Exiting);
    if (!ExitingPlayer)
    {
        return;
    }

    // 배열 내 인덱스를 찾습니다.
    int32 Index = PlayerQueue.Find(ExitingPlayer);
    if (Index != INDEX_NONE)
    {
        PlayerQueue.RemoveAtSwap(Index);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("%s 제거하려는 클라이언트 큐에 존재하지않음"),ANSI_TO_TCHAR(__FUNCTION__));
        check(false);
    }
    UE_LOG(LogTemp, Display, TEXT("%s PlayerQueue.NUM : %d , isMatchEntered : %d"),ANSI_TO_TCHAR(__FUNCTION__),PlayerQueue.Num(), isMatchEntered);

    //한명남아있다면 해당 플레이어 우승처리 후 게임종료
    if (isMatchEntered && PlayerQueue.Num() == 1)
    {

        APlayerController* pc = PlayerQueue[0];
        ACharacter* tmpCharacter = pc->GetCharacter();

        ASOTestCharacter* tmpSnake = Cast<ASOTestCharacter>(tmpCharacter);
        tmpSnake->OnGameOver();

    }
    if (isMatchEntered && PlayerQueue.Num() == 0)
        ExitGameServer();
}

void USOServerSubsystem::ExitGameServer()
{
    UE_LOG(LogTemp, Display, TEXT("%s" ), ANSI_TO_TCHAR(__FUNCTION__));

        // 기타 필요한 종료 전 작업을 수행한 후...
        FGenericPlatformMisc::RequestExit(true);
}

FPlayerSettings USOServerSubsystem::GetPlayerSetting(uint32 id)
{
    if(!playerSettingMap.Contains(id))
    {
        FPlayerSettings defaultSetting;
        defaultSetting.PlayerName =FText::FromString( TEXT("Default123"));
        defaultSetting.PlayerMaterialIdx = 0;
        return defaultSetting;
    }
    return playerSettingMap[id];
}
