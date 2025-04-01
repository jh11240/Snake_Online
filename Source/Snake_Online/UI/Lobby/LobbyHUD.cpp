// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Lobby/LobbyHUD.h"
#include <Runtime/UMG/Public/BluePrint/WidgetLayoutLibrary.h>
#include "UI/Lobby/LobbyPlayerInfoWidget.h"
#include "Kismet/GameplayStatics.h"


ALobbyHUD::ALobbyHUD()
{
	{
		UClass* WidgetClass = LoadClass<ULobbyPlayerInfoWidget>(nullptr,
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Lobby/SOLobbyPlayerInfo.SOLobbyPlayerInfo_C'"));

		check(WidgetClass);
		lobbyInfoWidget = CreateWidget<ULobbyPlayerInfoWidget>(GetWorld(), WidgetClass);

	}
}

void ALobbyHUD::BeginPlay()
{
	Super::BeginPlay();
	check(lobbyInfoWidget);
	lobbyInfoWidget->AddToViewport();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableTouchEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
		PlayerController->bEnableTouchOverEvents = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}
