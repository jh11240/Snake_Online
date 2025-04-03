// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UFUNCTION(Server, reliable)
	void CToSAddPlayerQueue();
};
