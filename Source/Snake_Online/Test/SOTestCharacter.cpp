// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestCharacter.h"
#include "Utils/SOUtils.h"

// Sets default values
ASOTestCharacter::ASOTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	{
			static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("/Script/Engine.DataTable'/Game/Test/Datatable/Snake.Snake'"));
			check(Asset.Object);
			UDataTable* SnakeDataTable = Asset.Object;

			// Row 순회하며 Material 정보 저장
			TArray<FName> RowNames = SnakeDataTable->GetRowNames();
			for (const FName& RowName : RowNames)
			{
				FSnakeTableRow* Row = SnakeDataTable->FindRow<FSnakeTableRow>(RowName, TEXT("Row Lookup"));
				if (Row && Row->Material) // Material이 존재하는 경우
				{
					Materials.Add(Row->Material); // Materials 배열에 추가
				}
			}

			SnakeDataTableRowHandle.DataTable = SnakeDataTable;
			SnakeDataTableRowHandle.RowName = "Green";

	}

	//복제 properties 처리
	{
		bReplicates = true;
		SetReplicateMovement(true);
		NetCullDistanceSquared = 100000000.0f;
		bAlwaysRelevant = true;
		bUseControllerRotationYaw = false;
	}

	//static mesh component 생성 - 머리 mesh 부분
	{
		//머리 컴퍼넌트
		HeadComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		HeadComponent->SetupAttachment(RootComponent);
		HeadComponent->ComponentTags.Add(SO::TagName::Head);
		HeadComponent->SetGenerateOverlapEvents(true);
		HeadComponent->SetCollisionProfileName(SO::CollisionProfileName::Head);
		HeadComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ThisClass::OnHeadOverlap);
		HeadComponent->SetIsReplicated(true);


		//SnakeStaticMeshComponent->SetEnableGravity(false);
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	//spring Arm이랑 camera 처리
	{
		SpringArm->ProbeSize = 5.0;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritRoll = false;

		const FRotator Rotation = FRotator(0., 0., 0.);
		const FVector Translation = FVector(0., 0., 2000.0);
		FTransform SpringArmTransform = FTransform(Rotation, Translation, FVector::OneVector);
		SpringArm->SetRelativeTransform(SpringArmTransform);
	}
	{
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		Camera->SetupAttachment(SpringArm);
		const FVector Translation = FVector(0., 0., 0.0);
		FQuat RotationQuat = FQuat(FRotator(-90., 0., 0.));
		FTransform CameraTransform = FTransform(RotationQuat, Translation, FVector::OneVector);
		Camera->SetRelativeTransform(CameraTransform);
	}

	//캡슐컴퍼넌트랑 기본 mesh 꺼주기
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetEnableGravity(false);
		GetCharacterMovement()->GravityScale = 0;
		GetMesh()->SetEnableGravity(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void ASOTestCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void ASOTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UpdatePawnDataTable();
	if (HasAuthority())
	{
		for (int i = 0; i < InitialSnakeBodyCnt; i++)
		AddBody();
	}
	// 클라중 내 클라만(ROLE_AutonomousProxy)
	/*else if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		
	}*/
}

// Called to bind functionality to input
void ASOTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASOTestCharacter::SetSnakeMaterial(int32 materialIdx)
{
	UMaterialInterface* materialToSet = Materials[materialIdx];
	CToSSetMaterial(materialToSet);
}

void ASOTestCharacter::OnHeadOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority()) {
		//벽에 닿았을 때
		if (OtherComp->ComponentHasTag(SO::TagName::Wall))
		{

		UE_LOG(LogTemp, Warning, TEXT("GameOver"));
		}
		else {
		UE_LOG(LogTemp, Warning, TEXT("asdlfkjasdf"));

		}

	}

}

void ASOTestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ReplicatedMaterial);
}

void ASOTestCharacter::UpdatePawnDataTable()
{
	if (SnakeDataTableRowHandle.IsNull()) { return; }
	FSnakeTableRow* Data = SnakeDataTableRowHandle.GetRow<FSnakeTableRow>(TEXT("Snake"));
	if (!Data) { return; }

	SnakeData = Data;
	{
		UCharacterMovementComponent* Movement = GetCharacterMovement();
		//Movement->RotationRate = CharacterData->RotationRate;
		Movement->bOrientRotationToMovement = true;
		Movement->GetNavAgentPropertiesRef().bCanCrouch = true;
		Movement->MaxWalkSpeed = SnakeData->MovementMaxSpeed;
		Movement->SetIsReplicated(true);
	}

	{
	
		HeadComponent->SetStaticMesh(SnakeData->StaticMesh);

		// ACharacter::PostInitializeComponents() 시점에 초기에 설정된 Mesh의 RelativeLocation, RelativeRotation을 받아와서
		// CharacterMovementComponent에서 사용하고 있음.
		// 우리는 데이터를 받아와서 적용하고 있기 때문에 한번 더 설정 필요
		// * 이부분을 뺴면 내 Character는 정상적으로 보이지만, 다른 플레이어의 Character는 이동되기전 위치 정보로 처리됨
		// 이 데이터를 통해서 클라 UCharacterMovementComponent::SmoothClientPosition_UpdateVisuals() 에서 Mesh의 위치를 바꾸고 있음.
		// (Mesh->SetRelativeLocationAndRotation(NewRelTranslation, NewRelRotation, false, nullptr, GetTeleportType());)
		CacheInitialMeshOffset(SnakeData->MeshTransform.GetTranslation(), SnakeData->MeshTransform.GetRotation().Rotator());
	}
}

void ASOTestCharacter::AddBody()
{

	 // 새 Body Segment 생성
	UStaticMeshComponent* NewBodySegment = NewObject<UStaticMeshComponent>(this);

	// 초기 설정
	NewBodySegment->SetCollisionProfileName(SO::CollisionProfileName::Body); // Body 충돌 프로파일
	NewBodySegment->SetGenerateOverlapEvents(true); // Overlap 이벤트 활성화
	NewBodySegment->SetStaticMesh(SnakeData->StaticMesh); // 사용할 Mesh를 설정
	NewBodySegment->RegisterComponent(); // 컴포넌트 등록

	// BodyComponents 배열에 추가
	BodyComponents.Add(NewBodySegment);
	NewBodySegment->SetIsReplicated(true);

	#pragma region 부모-자식 관계 설정 (이미 등록된 상태이므로 AttachToComponent 사용)
	if (BodyComponents.Num() > 1)
	{
		UStaticMeshComponent* PreviousSegment = BodyComponents[BodyComponents.Num() - 2];
		NewBodySegment->AttachToComponent(HeadComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	else
	{
		NewBodySegment->AttachToComponent(HeadComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	#pragma endregion

	#pragma region 새 노드의 위치 설정
	FVector SpawnLocation = FVector::ZeroVector;
	if (BodyComponents.Num() > 1)
	{
		UStaticMeshComponent* PreviousSegment = BodyComponents[BodyComponents.Num() - 2];
		FVector PreviousLocation = PreviousSegment->GetRelativeLocation();
		float BodyDiameter = PreviousSegment->Bounds.BoxExtent.Y * 2.0f; // 이전 Body의 지름 계산
		SpawnLocation = PreviousLocation - FVector(0.0f, BodyDiameter, 0.0f); // -Y축으로 이동
	}
	else
	{
		SpawnLocation = HeadComponent->GetRelativeLocation() - FVector(0.0f, SnakeData->StaticMesh->GetBounds().BoxExtent.Y * 2.0f, 0.0f);
	}

	NewBodySegment->SetRelativeLocation(SpawnLocation);
#pragma endregion

	UE_LOG(LogTemp, Warning, TEXT("Added new body segment! Total body count: %d"), BodyComponents.Num());
}

void ASOTestCharacter::OnRep_Material()
{
	HeadComponent->SetMaterial(0, ReplicatedMaterial);
	for (UStaticMeshComponent* elem : BodyComponents)
	{
		if (!elem || !elem->IsRegistered())
		{
			UE_LOG(LogTemp, Warning, TEXT("Component is not registered or nullptr"));
			continue;
		}
		elem->SetMaterial(0, ReplicatedMaterial);
	}
}

void ASOTestCharacter::CToSSetMaterial_Implementation(UMaterialInterface* NewMaterial)
{
	//서버는 메테리얼 정보 필요 x 클라에서 처리해야함
	ReplicatedMaterial = NewMaterial;
}

