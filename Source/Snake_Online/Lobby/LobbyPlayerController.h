// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"
struct FPlayerSettings;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void OnPossess(APawn* aPawn) override;
public:
	UFUNCTION(Client, reliable)
	void SToCSetPlayerUniqueID(uint32 UniqueID);

	UFUNCTION(Server, reliable)
	void CToSAddPlayerQueue();

	UFUNCTION(Server, reliable)
	void CToSSetPlayerSetting(uint32 id, FPlayerSettings playerSettings);
};
