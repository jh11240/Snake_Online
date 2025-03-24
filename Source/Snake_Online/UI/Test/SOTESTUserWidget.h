// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SOTESTUserWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API USOTESTUserWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

    // NativeConstruct 재정의
    virtual void NativeConstruct() override;

    // 위젯 블루프린트와 연결될 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* loginButton;

    // 버튼 클릭 이벤트 함수 선언
    UFUNCTION()
    void OnMyButtonClicked();
};
