// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SOServerTestActor.generated.h"

UCLASS()
class SNAKE_ONLINE_API ASOServerTestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASOServerTestActor();

	virtual void PostInitProperties() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
public:
	// 이 Actor또는 Component에 Owner가 있으면
// - 해당 Owner Clinet에서 함수가 실행됨
// Owner가 없으면
// - 서버에서 함수가 실행됨
	UFUNCTION(Client, Reliable)
	void ServerToClient(AActor* Actor, int32 Value);
	// 클라에서 Owner 권한이 부여되어 있으면 서버에서 실행
	UFUNCTION(Server, Reliable)
	void ClientToServer(AActor* Actor, int32 Value);

	// 클라에서 호출하면 그 클라에서만 실행
	// 서버에서 호출하면 모든 클라에서 실행
	UFUNCTION(NetMulticast, Reliable)
	void MulticastFunction(AActor* Actor, int32 Value);

protected:
	UPROPERTY()
	UStaticMeshComponent* MeshComponent = nullptr;


protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 서버에서 바꾸면, 클라에서 실행
	UPROPERTY(ReplicatedUsing = OnRep_Rotation)
	FRotator ActorRotation;

	UFUNCTION()
	void OnRep_Rotation();
};
