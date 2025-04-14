// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TitleHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWidgetAddedToViewport, UUserWidget*, Widget);
class UTitleUserWidget;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API ATitleHUD : public AHUD
{
	GENERATED_BODY()
public:
	ATitleHUD();
	UTitleUserWidget* GetTitleInfoWidget() { return TitleInfoWidget; }
protected:
	UTitleUserWidget* TitleInfoWidget = nullptr;

protected:
	virtual void BeginPlay() override;
public:
	FOnWidgetAddedToViewport OnWidgetAddedToViewport;
};
