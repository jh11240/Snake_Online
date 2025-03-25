// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "SnakeData.generated.h"

USTRUCT()
struct SNAKE_ONLINE_API FSnakeTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Snake")
	UStaticMesh* StaticMesh = nullptr;

	UPROPERTY(EditAnywhere, Category = "Snake")
	FTransform MeshTransform = FTransform::Identity;

public: // Material
    UPROPERTY(EditAnywhere, Category = "Snake")
    UMaterialInterface* Material = nullptr;

public: // Movement
	UPROPERTY(EditAnywhere, Category = "Pawn|Movement")
	float MovementMaxSpeed = 400.f;

};