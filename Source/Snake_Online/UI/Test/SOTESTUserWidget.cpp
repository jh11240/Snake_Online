// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Test/SOTESTUserWidget.h"
#include "Components/Button.h"
#include "GameFramework/PlayerController.h"
#include "Utils/NetworkUtils.h"

void USOTESTUserWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 버튼과 클릭 이벤트를 연결
    if (loginButton) // Null 체크
    {
        loginButton->OnClicked.AddDynamic(this, &USOTESTUserWidget::OnMyButtonClicked);
    }
}

void USOTESTUserWidget::OnMyButtonClicked()
{
    // PlayerController를 가져오기
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        // 로컬 컨트롤러인지 확인 후 서버로 연결
        if (PlayerController->IsLocalController())
        {
            PlayerController->ClientTravel(SO::NetworkUtils::GetServerURL(), TRAVEL_Absolute);
        }
    }
    if (loginButton) // Null 체크
    loginButton->SetVisibility(ESlateVisibility::Collapsed);
}
