// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestPlayerController.h"
#include "Blueprint/UserWidget.h"

ATestPlayerController::ATestPlayerController()
{
    //// 블루프린트 클래스를 찾고 TSubclassOf에 할당
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/SOLogin.SOLogin_C'"));
    if (WidgetClassFinder.Succeeded())
    {
        LobbyWidget = WidgetClassFinder.Class;
        UE_LOG(LogTemp, Warning, TEXT("UserWidgetClass successfully assigned."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find UserWidgetClass!"));
    }
}

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();
    //TODO: Map처리
    if (IsLocalController()) // 로컬 플레이어 컨트롤러인지 확인
    {
        if (LobbyWidget && createdLobbyWidget == nullptr) // UMG 블루프린트 클래스 확인
        {
            createdLobbyWidget =Cast<UUserWidget>( CreateWidget<UUserWidget>(GetWorld(), LobbyWidget));
            if (createdLobbyWidget)
            {
                createdLobbyWidget->AddToViewport(); // 화면에 위젯 표시
            }
        }
    }
}
