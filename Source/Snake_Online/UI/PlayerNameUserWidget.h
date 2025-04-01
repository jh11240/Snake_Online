// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerNameUserWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API UPlayerNameUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (bindwidget))
	UTextBlock* NameDisplayText;

	UFUNCTION()
	void SetNameText(FText inName);
};
