// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/SOServerSubsystem.h"
#include "Utils/NetworkUtils.h"
void USOServerSubsystem::AddPlayerToQueue(APlayerController* Player)
{
    if (!Player) return;
    PlayerQueue.Add(Player);

    TryMatchPlayers();
}

void USOServerSubsystem::TryMatchPlayers()
{
    if (PlayerQueue.Num() < 2)
        return;

    APlayerController* Player1 = PlayerQueue[0];
    APlayerController* Player2 = PlayerQueue[1];

    //큐 비워주고
    PlayerQueue.RemoveAt(0);
    PlayerQueue.RemoveAt(0);

    //TODO: 임시로 두명 접속되는지 확인하기
    FString TravelCommand = SO::NetworkUtils::GetGameMapURL() + TEXT("?listen");
    //GetWorld()->ServerTravel(TravelCommand);

    Player1->ClientTravel(TravelCommand, TRAVEL_Absolute);
    Player2->ClientTravel(TravelCommand, TRAVEL_Absolute);
}
