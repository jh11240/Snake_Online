// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestLoginModeBase.h"
#include "UI/Lobby/LobbyHUD.h"
#include "Lobby/LobbyPlayerController.h"
ASOTestLoginModeBase::ASOTestLoginModeBase()
{
	HUDClass = ALobbyHUD::StaticClass();
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	//bUseSeamlessTravel = true;

}

