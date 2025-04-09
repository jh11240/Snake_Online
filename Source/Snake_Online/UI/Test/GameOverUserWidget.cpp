// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Test/GameOverUserWidget.h"
#include "Utils/NetworkUtils.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"


void UGameOverUserWidget::NativeConstruct()
{
	if (ReturnButton)
		ReturnButton->OnClicked.AddUniqueDynamic(this, &UGameOverUserWidget::ReturnToLobby);
}

void UGameOverUserWidget::ReturnToLobby()
{
    FString TravelCommand = SO::NetworkUtils::GetTitleURL();
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // 로컬 컨트롤러인지 확인 후 서버로 연결
        if (PlayerController->IsLocalController())
        {
            //PlayerController->ClientTravel(TravelCommand, TRAVEL_Absolute);
            UGameplayStatics::OpenLevel(GetWorld(), FName(TravelCommand));
        }
    }
}
#define LOCTEXT_NAMESPACE "GameOverWidget"
void UGameOverUserWidget::SetPlaceText(uint32 place)
{
    FText comment = FText::Format(LOCTEXT("PlaceFormat", "{0}등"), place);
    TextPlace->SetText(comment);
}

#undef LOCTEXT_NAMESPACE