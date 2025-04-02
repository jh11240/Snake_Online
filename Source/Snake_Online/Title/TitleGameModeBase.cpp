// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TitleGameModeBase.h"
#include "Title/TitleHUD.h"
ATitleGameModeBase::ATitleGameModeBase()
{
	HUDClass = ATitleHUD::StaticClass();
}
