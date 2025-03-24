// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API ATestPlayerController : public APlayerController
{
	GENERATED_BODY()
	ATestPlayerController();
public:
	virtual void BeginPlay() override;
	

protected:
	TSubclassOf<UUserWidget> LobbyWidget;
	UUserWidget* createdLobbyWidget;
};
