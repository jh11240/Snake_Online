// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestCharacter.h"

// Sets default values
ASOTestCharacter::ASOTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASOTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASOTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASOTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

