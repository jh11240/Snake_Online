// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverUserWidget.generated.h"
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API UGameOverUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	//랜덤 매칭 버튼
	UPROPERTY(meta = (BindWidget))
	UButton* ReturnButton;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TextPlace;
protected:

	UFUNCTION()
	void ReturnToLobby();
	UFUNCTION()
	void SetPlaceText(uint32 place);
};
