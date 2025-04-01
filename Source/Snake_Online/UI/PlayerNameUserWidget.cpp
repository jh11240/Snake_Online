// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerNameUserWidget.h"
#include "Components/TextBlock.h"

void UPlayerNameUserWidget::SetNameText(FText inName)
{
	NameDisplayText->SetText(inName);
}
