// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

class UUserWidget;
class InputAction;
struct FInputActionValue;
class UInputMappingContext;
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
	virtual void SetupInputComponent() override;
protected:
	void OnMove(const FInputActionValue& InputActionValue);

protected:
	TSubclassOf<UUserWidget> LobbyWidget;
	UUserWidget* createdLobbyWidget;
	UInputMappingContext* IMC_Move= nullptr;
};
