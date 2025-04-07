// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SOServerSubsystem.h"
#include "Utils/NetworkUtils.h"
#include "Net/UnrealNetwork.h"


void USOServerSubsystem::AddPlayerToQueue(APlayerController* Player)
{
    UE_LOG(LogTemp, Warning, TEXT("AddPlayerToQueue 서버에서 실행됨!"));

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
        if (PlayerQueue.Num() < 2)
            return;

        APlayerController* Player1 = PlayerQueue[0];
        APlayerController* Player2 = PlayerQueue[1];

        //큐 비워주고
        PlayerQueue.RemoveAt(0);
        PlayerQueue.RemoveAt(0);
        ///Script/Engine.World'/Game/Test/Test.Test'
        //TODO: 임시로 두명 접속되는지 확인하기
        

        FString TravelCommand = SO::NetworkUtils::GetGameMapURL();
// UE_EDITOR

        GetWorld()->ServerTravel(TravelCommand);

        //Player1->ClientTravel(TravelCommand, TRAVEL_Absolute);
        //Player2->ClientTravel(TravelCommand, TRAVEL_Absolute);
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
