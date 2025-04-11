// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SnakeFood.h"
#include "Test/SOTestCharacter.h"
#include "Utils/SOUtils.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASnakeFood::ASnakeFood()
{
 	
	bReplicates = true;
	SetReplicateMovement(true);
	bAlwaysRelevant = true;
#pragma region 둘러싸는 Collider 세팅
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Collider->SetCollisionProfileName(SO::CollisionProfileName::Food);
	Collider->SetIsReplicated(true);
	RootComponent = Collider;
#pragma endregion

#pragma region StaticMeshComponent 세팅
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetIsReplicated(true);

	// StarterContent의 Sphere 메쉬를 찾습니다.
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(
		TEXT("/Script/Engine.StaticMesh'/Game/Mesh/Apple/Red_Apple_tgzoahbpa_Low.Red_Apple_tgzoahbpa_Low'")
	);

	if (SphereMesh.Succeeded() && SphereMesh.Object)
	{
		// 메쉬 설정
		StaticMeshComponent->SetStaticMesh(SphereMesh.Object);
		StaticMeshComponent->SetRelativeScale3D(FVector(15, 15, 15));
	}
#pragma endregion

	Collider->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASnakeFood::OnOverlap);

}

// Called when the game starts or when spawned
void ASnakeFood::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASnakeFood::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, FoodScore);
}

// Called every frame
void ASnakeFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASnakeFood::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		if (OtherComp->ComponentHasTag(SO::TagName::Head))
		{
			ASOTestCharacter* character = Cast<ASOTestCharacter>(OtherActor);
			if (character)
			{
				EatFood(character);
			}
		}
		//현재 그냥 테스트할때 이리저리 스폰된느거 찾기 귀찮아서 한군데에서 계속 냠냠중
		Destroy();
	}
}

void ASnakeFood::EatFood(ASOTestCharacter* character)
{
	character->AddConsumedFood(FoodScore);
}

