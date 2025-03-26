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
	virtual void OnRep_Pawn() override;
	UUserWidget* GetLobbyWidget() {return createdLobbyWidget;	};
protected:
	UFUNCTION(Server, Unreliable)
	void CToSMove(const FVector& Direction, float Value);
	
	void OnMove(const FInputActionValue& InputActionValue);

protected:
	TSubclassOf<UUserWidget> LobbyWidget;
	UUserWidget* createdLobbyWidget;
	UInputMappingContext* IMC_Move= nullptr;

//private:
//	//On Move property
//	double LastMoveTime = 0.0;
//	const double MoveInterval = 0.1;
//	double CurrentTime = 0.0;

};
