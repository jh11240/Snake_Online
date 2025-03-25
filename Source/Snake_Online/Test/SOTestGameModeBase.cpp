// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestGameModeBase.h"
#include "Test/TestPlayerController.h"
#include "Test/SOTestCharacter.h"

ASOTestGameModeBase::ASOTestGameModeBase()
{
	PlayerControllerClass = ATestPlayerController::StaticClass();
	DefaultPawnClass = ASOTestCharacter::StaticClass();
}
