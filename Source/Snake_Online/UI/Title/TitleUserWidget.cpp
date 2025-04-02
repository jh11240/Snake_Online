// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Title/TitleUserWidget.h"
#include "Components/Button.h"
#include "Utils/NetworkUtils.h"
#include "GameFramework/PlayerController.h"


void UTitleUserWidget::NativeConstruct()
{
    if(loginButton)
    loginButton->OnClicked.AddUniqueDynamic(this, &UTitleUserWidget::ServerConnect);
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
            PlayerController->ClientTravel(SO::NetworkUtils::GetServerURL(), TRAVEL_Absolute);
        }
    }
}
