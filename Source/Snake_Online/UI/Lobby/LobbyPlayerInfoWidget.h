// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerInfoWidget.generated.h"

class UButton;
class UCircularThrobber;
class UTextBlock;
class UEditableText;
class  ALobbySnakeActor;
/**
 * 
 */
UCLASS()
class SNAKE_ONLINE_API ULobbyPlayerInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

    //랜덤 매칭 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* MatchStartButton;

    //매칭 시작시 띄울 throbber과 텍스트
    UPROPERTY(meta = (BindWidget))
    UCircularThrobber* MatchingWaitThrobber;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* MatchingWaitText;

    //뱀 material 변경 버튼
    UPROPERTY(meta = (BindWidget))
    UButton* NextMatButton;
    UPROPERTY(meta = (BindWidget))
    UButton* PrevMatButton;

    //이름 세팅
    UPROPERTY(meta = (BindWidget))
    UEditableText* NameText;
    UPROPERTY(meta = (BindWidget))
    UButton* NameSelectButton;

    FText Name;
    ALobbySnakeActor* LobbySnake;
public :
    void Begin();
protected:
    UFUNCTION()
    void MatchStart();

    UFUNCTION()
    void NextMaterialBtnPressed();
    
    UFUNCTION()
    void PrevMaterialBtnPressed();

    UFUNCTION()
    void SetName();
};
