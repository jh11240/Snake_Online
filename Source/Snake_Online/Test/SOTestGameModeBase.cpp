// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestGameModeBase.h"
#include "Test/TestPlayerController.h"

ASOTestGameModeBase::ASOTestGameModeBase()
{
	PlayerControllerClass = ATestPlayerController::StaticClass();
}
