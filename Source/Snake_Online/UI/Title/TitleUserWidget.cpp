// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleUserWidget.h"
#include "Components/Button.h"
#include "Utils/NetworkUtils.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"


void UTitleUserWidget::NativeConstruct()
{
    if (loginButton) {
        UE_LOG(LogTemp, Display, TEXT("%s, login button DeActivate"), ANSI_TO_TCHAR(__FUNCTION__));
        loginButton->OnClicked.AddUniqueDynamic(this, &UTitleUserWidget::ServerConnect);
    SetLoginBtnActivate(false);
    }

}

void UTitleUserWidget::ServerConnect()
{
	FString TravelCommand = SO::NetworkUtils::GetServerURL();
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // 로컬 컨트롤러인지 확인 후 서버로 연결
        if (PlayerController->IsLocalController())
        {
            UGameplayStatics::OpenLevel(GetWorld(), FName(SO::NetworkUtils::GetServerURL()));
            //PlayerController->ClientTravel(SO::NetworkUtils::GetServerURL(), TRAVEL_Absolute);
        }
    }
}

void UTitleUserWidget::SetLoginBtnActivate(bool active)
{
    if (active)
    {
        UE_LOG(LogTemp, Display, TEXT("%s, login button activate"),ANSI_TO_TCHAR(__FUNCTION__));
        loginButton->SetVisibility(ESlateVisibility::Visible);
    }
    else
        loginButton->SetVisibility(ESlateVisibility::Hidden);
}
