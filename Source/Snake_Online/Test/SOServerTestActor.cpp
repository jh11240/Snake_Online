// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOServerTestActor.h"
#include "SOServerTestActor.h"
#include "Net/UnrealNetwork.h"
#include "UnrealClient.h"


// Sets default values
ASOServerTestActor::ASOServerTestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true; // Multicast 무조건 날릴려면 true로 설정

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent0"));
	RootComponent = MeshComponent;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshCube(
		TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	MeshComponent->SetStaticMesh(StaticMeshCube.Object);
}

void ASOServerTestActor::PostInitProperties()
{
	Super::PostInitProperties();
	SetRemoteRoleForBackwardsCompat(ROLE_AutonomousProxy);
}

// Called when the game starts or when spawned
void ASOServerTestActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASOServerTestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ENetRole NetRole = GetLocalRole();

	if (HasAuthority())
	{
		AddActorWorldRotation(FRotator(0.f, 360.f * DeltaTime, 0.f));
		ActorRotation = GetActorRotation();

		if (GetWorld()->GetFirstPlayerController())
		{
			SetOwner(GetWorld()->GetFirstPlayerController());
		}
		else
		{
			SetOwner(nullptr);
		}
	}
	else
	{

	}

}

void ASOServerTestActor::ServerToClient_Implementation(AActor* Actor, int32 Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Client RPC: %d"), Value);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::FromInt(Value));

	ENetRole NetRole = GetLocalRole();
	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server!"));
	}
}

void ASOServerTestActor::ClientToServer_Implementation(AActor* Actor, int32 Value)
{
	UE_LOG(LogTemp, Warning, TEXT("ClientToServer_Implementation called on server!"));

}

void ASOServerTestActor::MulticastFunction_Implementation(AActor* Actor, int32 Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Multicast function called on all clients and server! : %d"), Value);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::FromInt(Value));

	if (!HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Server!"));
	}
}

void ASOServerTestActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ActorRotation을 리플리케이션하도록 등록
	DOREPLIFETIME(ThisClass, ActorRotation);
}

void ASOServerTestActor::OnRep_Rotation()
{
	SetActorRotation(ActorRotation);

}

