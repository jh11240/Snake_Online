// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LobbySnakeActor.generated.h"
class UWidgetComponent;

UCLASS()
class SNAKE_ONLINE_API ALobbySnakeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALobbySnakeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
protected:
	UStaticMesh* SphereMesh = nullptr;

	UPROPERTY(EditAnywhere)
	UWidgetComponent* NameTextComponent = nullptr;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* HeadComponent;

	UPROPERTY(VisibleAnywhere)
	TArray<UStaticMeshComponent*> BodyComponents;
	float BodyDiameter = 0;

protected:
	//Material
	UPROPERTY(EditAnywhere, Category = "Materials")
	TArray<UMaterialInterface*> Materials;
	int32 MaterialSize = 0;

	void SetSnakeMaterial(int32 materialIdx);
	int32 curIdx=0;

public:
	void SetNextSnakeMaterial();
	void SetPrevSnakeMaterial();
	void SetNameText(FText InName);
protected:
	//몸통 위치 갱신
	UFUNCTION()
	void SetBodyLocation();

	//RPC -> 서버 함수로
	UFUNCTION()
	void MoveBodySegments(float DeltaTime);

	TArray<FVector> BodyComponentsLoc;

	float BodyMoveRefreshRate = .2f;
	float curSec = 0;
	UPROPERTY(EditAnywhere)
	float moveSpeed = 750.f;

	float RotationSpeed = 90.f;
	float RotateAngle = 0.f;
	UPROPERTY(EditAnywhere)
	float RotateRadius = 500.f;
public:
	void AddBody();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


};
