// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"
#include "Subsystem/SOServerSubsystem.h"

void ALobbyPlayerController::CToSAddPlayerQueue_Implementation()
{
    USOServerSubsystem* MatchmakingSubsystem = GetGameInstance()->GetSubsystem<USOServerSubsystem>();
    UE_LOG(LogTemp, Warning, TEXT("USOServerSubsystem 서버에서 정상적으로 가져옴!"));
    if (MatchmakingSubsystem)
    {
        MatchmakingSubsystem->AddPlayerToQueue(this);
    }
}