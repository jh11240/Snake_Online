// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleUserWidget.generated.h"
class UButton;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API UTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

    // NativeConstruct 재정의
    virtual void NativeConstruct() override;

    virtual void ServerConnect();
    // 위젯 블루프린트와 연결될 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* loginButton;
};
