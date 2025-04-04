// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyPlayerInfoWidget.h"
#include "Components/CircularThrobber.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "EngineUtils.h" 
#include "Lobby/LobbyPlayerController.h"
#include "Lobby/LobbySnakeActor.h"
#include "Subsystem/SOServerSubsystem.h"

void ULobbyPlayerInfoWidget::NativeConstruct()
{
    if (MatchStartButton) // Null 체크
    {
        MatchStartButton->OnClicked.AddDynamic(this, &ULobbyPlayerInfoWidget::MatchStart);
    }
    if (NextMatButton)
    {
        NextMatButton->OnClicked.AddDynamic(this, &ULobbyPlayerInfoWidget::NextMaterialBtnPressed);
    }
    if (PrevMatButton)
    {
        PrevMatButton->OnClicked.AddDynamic(this, &ULobbyPlayerInfoWidget::PrevMaterialBtnPressed);
    }
    if (NameSelectButton)
    {
        NameSelectButton->OnClicked.AddDynamic(this, &ULobbyPlayerInfoWidget::SetName);

    }
    MatchingWaitThrobber->SetVisibility(ESlateVisibility::Hidden);

}


void ULobbyPlayerInfoWidget::Begin()
{
    UWorld* World = GetWorld();
    if (!World) return;

    for (TActorIterator<ALobbySnakeActor> It(World); It; ++It)
    {
        ALobbySnakeActor* FoundActor = *It;
        if (FoundActor)
        {
            LobbySnake = FoundActor;
        }
    }
    pc = Cast<ALobbyPlayerController> (World->GetFirstPlayerController());
}

void ULobbyPlayerInfoWidget::MatchStart()
{
    MatchingWaitThrobber->SetVisibility(ESlateVisibility::Visible);
    MatchingWaitText->SetText(FText::FromString(TEXT("랜덤 매칭 중...")));

#pragma region player setting 중
    uint32 id=0;
        if (UGameInstance* gameInstance = GetGameInstance())
        {
            USOServerSubsystem* serverSystem = gameInstance->GetSubsystem<USOServerSubsystem>();
            if (serverSystem)
            {
                id = serverSystem->GetClientID();
                UE_LOG(LogTemp, Display, TEXT(" 매치 시작 : 현재 클라이언트 id : %d"),id);
            }
        }
        FPlayerSettings playerSetting ;
        if (Name.IsEmpty()) Name = FText::FromString("Default012");
        playerSetting.PlayerName = Name;
        playerSetting.PlayerMaterialIdx = LobbySnake->GetSnakeMaterial();

        pc->CToSSetPlayerSetting(id,playerSetting);
#pragma endregion
    pc->CToSAddPlayerQueue();
    UE_LOG(LogTemp, Warning, TEXT("Match making started!"));
}

void ULobbyPlayerInfoWidget::NextMaterialBtnPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Next Material started!"));
    if (!LobbySnake)
        return;
    LobbySnake->SetNextSnakeMaterial();
}

void ULobbyPlayerInfoWidget::PrevMaterialBtnPressed()
{
    UE_LOG(LogTemp, Warning, TEXT("Prev Material started!"));
    if (!LobbySnake)
        return;
    LobbySnake->SetPrevSnakeMaterial();
}

void ULobbyPlayerInfoWidget::SetName()
{
    Name = NameText->GetText();
    if (!LobbySnake)
        return;
    LobbySnake->SetNameText(Name);
}
