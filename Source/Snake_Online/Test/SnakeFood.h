// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "SnakeFood.generated.h"

class ASOTestCharacter;
UCLASS()
class SNAKE_ONLINE_API ASnakeFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASnakeFood();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Collider;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SetFoodScore(int32 inValue) { FoodScore = inValue; }
	int32 GetFoodScore() { return FoodScore; }
public:
	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent
		, AActor* OtherActor
		, UPrimitiveComponent* OtherComp
		, int32 OtherBodyIndex
		, bool bFromSweep
		, const FHitResult& SweepResult
	);
protected:
	// 먹이 점수 나타내는 변수
	// 서버에서 생성하지만 클라이언트에서도 이 값을 받아와 점수 표기해야하므로 복제
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Food")
	int32 FoodScore=1;

	virtual void EatFood(ASOTestCharacter* character);

};
