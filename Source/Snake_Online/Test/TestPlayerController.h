// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TestPlayerController.generated.h"

class UUserWidget;
class InputAction;
struct FInputActionValue;
class UInputMappingContext;
class ASOTestCharacter;
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
	UUserWidget* GetLobbyWidget() {return createdLobbyWidget;};

protected:
	UFUNCTION(Server, Unreliable)
	void CToSMove(const FVector& Direction, float Value);
	UFUNCTION(Server,reliable)
	void CToSSetPlayerInfo(ASOTestCharacter* playerCharacter,uint32 playerId);

	void OnMove(const FInputActionValue& InputActionValue);

	//혹시나 나중에 부스트 구현시 사용하려고
	float speedMultiplier = 1.f;
	float moveSpeed=100.f;
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
