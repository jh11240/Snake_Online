// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Test/SnakeData.h"
#include "Net/UnrealNetwork.h"
#include "SOTestCharacter.generated.h"



UCLASS()
class SNAKE_ONLINE_API ASOTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASOTestCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UFUNCTION()
	void SetSnakeMaterial(int32 materialIdx);
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void OnRep_UpdatePawnDataTable();
	UPROPERTY()
	UStaticMeshComponent* SnakeStaticMeshComponent;

protected:
	//Material 처리

	UPROPERTY(EditAnywhere, Category = "Materials")
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(ReplicatedUsing = OnRep_Material)
	UMaterialInterface* ReplicatedMaterial;

	UFUNCTION()
	void OnRep_Material();

	UFUNCTION(Server, Reliable)
	void CToSSetMaterial(UMaterialInterface* NewMaterial);


protected:
	//data table처리
	UPROPERTY(ReplicatedUsing = OnRep_UpdatePawnDataTable, EditAnywhere, meta = (RowType = "/Script/KDT3D.SnakeTableRow"))
	FDataTableRowHandle SnakeDataTableRowHandle;
	FSnakeTableRow* SnakeData = nullptr;
	
	/*TODO: 만약된다면 위에 미니맵 처리
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* MinimapSpriteComponent;*/
};
