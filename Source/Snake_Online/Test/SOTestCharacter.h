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
public:

	UFUNCTION()
	void OnHeadOverlap
	(
		UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex
		, bool bFromSweep
		, const FHitResult& SweepResult
	);
protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void UpdatePawnDataTable();

protected:
	//머리 몸통 관련
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* HeadComponent;

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<UStaticMeshComponent*> BodyComponents;

	UFUNCTION()
	void AddBody();

	//몸 지름
	float BodyDiameter = 0;
	int InitialSnakeBodyCnt=5;

	//현재 snake 방향 관련
	FVector Get8SideDir();
protected:
	//점수 처리

	UPROPERTY(ReplicatedUsing = OnRep_ConsumedFood )
	int ConsumedFood;

	//서버 - 이 변수만큼 음식먹으면 몸 길어짐
	int FoodCntToAddBody = 1;
	//서버 - 몸 길이 컨트롤 변수(마지막으로 몸 추가한 후,남은 먹이수 관리)
	int CurRemainingFoodCnt= 0;

	UFUNCTION()
	void OnRep_ConsumedFood();

public: 
	//먹이 먹었을 때
	UFUNCTION()
	void AddConsumedFood(int food);
	//지금은 아니지만 추후 부스트 구현시 필요
	UFUNCTION()
	void RemoveConsumedFood(int food) { ConsumedFood -= food; }
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
	//UPROPERTY(ReplicatedUsing = OnRep_UpdatePawnDataTable, EditAnywhere, meta = (RowType = "/Script/KDT3D.SnakeTableRow"))
	FDataTableRowHandle SnakeDataTableRowHandle;
	FSnakeTableRow* SnakeData = nullptr;
	
	/*TODO: 만약된다면 위에 미니맵 처리
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* MinimapSpriteComponent;*/
};
