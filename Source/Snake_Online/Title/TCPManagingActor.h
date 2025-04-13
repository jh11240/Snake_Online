// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TCPManagingActor.generated.h"

UCLASS()
class SNAKE_ONLINE_API ATCPManagingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATCPManagingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	FString serverRequestMsg = "UmVxdWVzdFNuYWtlR2FtZVNlcnZlcg==";
};
