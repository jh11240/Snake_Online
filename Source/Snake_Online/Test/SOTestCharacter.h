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

class UWidgetComponent;

UCLASS()
class SNAKE_ONLINE_API ASOTestCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASOTestCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//뱀 상태 초기화 -> playercontrolller onpossess에서 호출
	void InitSnake();

protected:
	//카메라와 스프링 암
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArm;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UFUNCTION()
	void UpdatePawnDataTable();

#pragma region 충돌, 게임오버, 무적 관련 처리
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

	UFUNCTION()
	void OnGameOver();
	UFUNCTION(Client, Reliable)
	void SToCGameOver();

	bool IsInvincible() { return isInvincible; }
	void SetInvincible(bool inVal) {if(HasAuthority())
		UE_LOG(LogTemp, Display, TEXT("%s, 무적 상태 변경"),ANSI_TO_TCHAR(__FUNCTION__));
		isInvincible = inVal; }
	float GetInvincibleTime() { return BeginInivincibleTime; }
protected:
	UPROPERTY(replicated)
	bool isInvincible;

	UPROPERTY()
	float BeginInivincibleTime = 2.5f;
	UPROPERTY()
	bool isGameOver=false;
	UPROPERTY()
	bool isPossessed=false;
#pragma endregion

#pragma region 이름 UI 관련
protected:
	UPROPERTY(EditAnywhere)
	UWidgetComponent* NameTextComponent = nullptr;

public: 
	UFUNCTION()
	void SetNameWidget(const FText& txtName);
	UPROPERTY(ReplicatedUsing = OnRep_NameUI)
	FText NameUIText;
	UFUNCTION()
	void OnRep_NameUI();
#pragma endregion

#pragma region 머리와 몸통 관리
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
#pragma endregion

#pragma region 점수처리 (Food 관련)
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
#pragma endregion

#pragma region Material 세팅 관련
public:
		UFUNCTION()
		void SetSnakeMaterial(int32 materialIdx);
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
	public:
	void ServerSetMaterial(uint32 NewMaterialIdx);
#pragma endregion

#pragma region 몸통 이동 관련
protected:
	//몸통 위치 갱신
	UFUNCTION()
	void SetBodyLocation();

	//RPC -> 서버 함수로
	UFUNCTION()
	void MoveBodySegments(float DeltaTime);
	
	TArray<FVector> BodyComponentsLoc;

	const float BodyMoveRefreshRate = .5f;;
	float curSec = 0;
	float moveSpeed = 100.f;

public:
	float GetMoveSpeed() { return moveSpeed; }
#pragma endregion

protected:
	//data table처리
	//UPROPERTY(ReplicatedUsing = OnRep_UpdatePawnDataTable, EditAnywhere, meta = (RowType = "/Script/KDT3D.SnakeTableRow"))
	FDataTableRowHandle SnakeDataTableRowHandle;
	FSnakeTableRow* SnakeData = nullptr;
	
	/*TODO: 만약된다면 위에 미니맵 처리
	UPROPERTY(VisibleAnywhere)
	UPaperSpriteComponent* MinimapSpriteComponent;*/

};
