// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class ULobbyPlayerInfoWidget;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()	
public:
	ALobbyHUD();

protected:
	ULobbyPlayerInfoWidget* lobbyInfoWidget = nullptr;

protected:
	virtual void BeginPlay() override;

};
