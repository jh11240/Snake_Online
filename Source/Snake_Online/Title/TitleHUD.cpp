// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TitleHUD.h"
#include "UI/Title/TitleUserWidget.h"
#include "Kismet/GameplayStatics.h"

ATitleHUD::ATitleHUD()
{
	{
		UClass* WidgetClass = LoadClass<UTitleUserWidget>(nullptr,
			TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Title/Title.Title_C'"));

		check(WidgetClass);
		TitleInfoWidget = CreateWidget<UTitleUserWidget>(GetWorld(), WidgetClass);

	}
}

void ATitleHUD::BeginPlay()
{
	Super::BeginPlay();
	check(TitleInfoWidget);
	TitleInfoWidget->AddToViewport();

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
