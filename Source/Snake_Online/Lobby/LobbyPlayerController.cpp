// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbyPlayerController.h"
#include "Subsystem/SOServerSubsystem.h"

void ALobbyPlayerController::CToSAddPlayerQueue_Implementation()
{

    USOServerSubsystem* MatchmakingSubsystem = GetGameInstance()->GetSubsystem<USOServerSubsystem>();
    UE_LOG(LogTemp, Warning, TEXT("플레이어 큐에 넣기 얍!"));
    if (MatchmakingSubsystem)
    {
        MatchmakingSubsystem->AddPlayerToQueue(this);
    }
}

void ALobbyPlayerController::SToCSetPlayerUniqueID_Implementation(uint32 UniqueID)
{
    if (UGameInstance* gameInstance = GetGameInstance())
    {
        USOServerSubsystem* serverSystem = gameInstance->GetSubsystem<USOServerSubsystem>();
        if (serverSystem)
        {
            serverSystem->SetClientID(UniqueID);
        }
    }

}

void ALobbyPlayerController::OnPossess(APawn* aPawn)
{
    Super::OnPossess(aPawn);

    if (UGameInstance* gameInstance = GetGameInstance())
    {
        USOServerSubsystem* serverSystem=  gameInstance->GetSubsystem<USOServerSubsystem>();
        if (serverSystem)
        {
            uint32 id = serverSystem->GetNewClientID();
            SToCSetPlayerUniqueID(id);
            UE_LOG(LogTemp, Display, TEXT("Lobby에서 클라이언트 번호 할당받음 : %d"),id);
        }
        else {
            check(false);
            return;
        }
    }
    else
    {
        check(false);
        return;
    }
}

void ALobbyPlayerController::CToSSetPlayerSetting_Implementation(uint32 id, FPlayerSettings playerSettings)
{
    USOServerSubsystem* MatchmakingSubsystem = GetGameInstance()->GetSubsystem<USOServerSubsystem>();
    UE_LOG(LogTemp, Warning, TEXT("PlayerSetting 서버에 저장!"));
    if (MatchmakingSubsystem)
    {
        MatchmakingSubsystem->SetPlayerSetting(id, playerSettings);
    }
}
