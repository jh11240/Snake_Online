// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestCharacter.h"
#include "Utils/SOUtils.h"

// Sets default values
ASOTestCharacter::ASOTestCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
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
		FVector headWorldLoc = HeadComponent->GetComponentLocation();
		BodyComponentsLoc.Add(headWorldLoc);
		for (int i = 0; i < InitialSnakeBodyCnt; i++)
		AddBody();
		if (BodyDiameter != 0)
			BodyMoveRefreshRate = BodyDiameter / moveSpeed;
		else
			BodyMoveRefreshRate = 1.f;
	}
	// 클라중 내 클라만(ROLE_AutonomousProxy)
	/*else if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		
	}*/
}

void ASOTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	#pragma region 몸통 분리 이동 로직
	if (HasAuthority())
	{
		MoveBodySegments(DeltaTime);
		if (curSec < BodyMoveRefreshRate) {
			curSec += DeltaTime;
			return;
		}
		SetBodyLocation();
		curSec = 0;
	}
#pragma endregion
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
	DOREPLIFETIME(ThisClass, BodyComponents);
	DOREPLIFETIME(ThisClass, ConsumedFood);
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
		BodyDiameter= HeadComponent->Bounds.BoxExtent.Y * 2.0f; // 이전 Body의 지름 계산

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
	NewBodySegment->ComponentTags.Add(SO::TagName::Body);
	NewBodySegment->SetIsReplicated(true);
	NewBodySegment->SetUsingAbsoluteLocation(true);

	#pragma region 부모-자식 관계 설정 (이미 등록된 상태이므로 AttachToComponent 사용)

	NewBodySegment->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	
	#pragma endregion

	#pragma region 새 노드의 위치 설정
	FVector SpawnLocation = FVector::ZeroVector;
	FVector CurrentDirection = Get8SideDir();

	if (BodyDiameter == 0)
	{
		BodyDiameter = SnakeData->StaticMesh->GetBounds().BoxExtent.Y * 2.0f;
	}
	if (BodyComponents.Num() > 1)
	{
		UStaticMeshComponent* PreviousSegment = BodyComponents[BodyComponents.Num() - 2];
		FVector PreviousLocation = PreviousSegment->GetComponentLocation();
		SpawnLocation = PreviousLocation - CurrentDirection*BodyDiameter; // -Y축으로 이동
		
	}
	else
	{
		SpawnLocation = HeadComponent->GetComponentLocation() - CurrentDirection*BodyDiameter;
	}

	NewBodySegment->SetWorldLocation(SpawnLocation);

	FVector bodyWorldLoc = NewBodySegment->GetComponentLocation();
	BodyComponentsLoc.Add(bodyWorldLoc);
#pragma endregion

	UE_LOG(LogTemp, Warning, TEXT("Added new body segment! Total body count: %d"), BodyComponents.Num());
}

//TODO: 이제 velocity가 아마 8방향으로 고정되어서 굳이안써도될듯
FVector ASOTestCharacter::Get8SideDir()
{
	FVector retDir = GetVelocity().GetSafeNormal();
	retDir.Z = 0.0f;
	if (!retDir.IsNearlyZero())
	{
		// X-Y 평면에서만 계산한다고 가정 (Z는 0)
		float AngleRad = FMath::Atan2(retDir.Y, retDir.X);
		//각도 구하기
		float AngleDeg = FMath::RadiansToDegrees(AngleRad);

		// 음수라면 360더해서 34분면 처리
		if (AngleDeg < 0.0f)
		{
			AngleDeg += 360.0f;
		}

		//이미 8방향이라면 return;
		float Remainder = FMath::Fmod(AngleDeg, 45.0f);
		//0에 가깝거나 45에가까우면 
		if (Remainder < KINDA_SMALL_NUMBER || (45.0f - Remainder) < KINDA_SMALL_NUMBER)
			return retDir;

		// 45도 단위로 나눈 값 (0 ~ 7)
		//22.5 더한 후에 버림 -> 22.5보다 크면 45로 측정하기위해
		int calAngle = FMath::FloorToInt((AngleDeg + 22.5f) / 45.f) % 8;

		switch (calAngle)
		{
		case 0:
			// 0도: 오른쪽 방향 (1, 0, 0)
			retDir = FVector(1.f, 0.f, 0.f);
			break;
		case 1:
			// 45도: 오른쪽 위 
			retDir = FVector(0.707f, 0.707f, 0.f);
			break;
		case 2:
			// 90도: 위쪽 (0, 1, 0)
			retDir = FVector(0.f, 1.f, 0.f);
			break;
		case 3:
			// 135도: 왼쪽 위 
			retDir = FVector(-0.707f,.707f, 0.f);

			break;
		case 4:
			// 180도: 왼쪽 ( -1, 0, 0 )
			retDir = FVector(-1.f, 0.f, 0.f);
			break;
		case 5:
			// 225도: 왼쪽 아래 
			retDir = FVector(-.707f, -.707f, 0.f);


			break;
		case 6:
			// 270도: 아래쪽 (0, -1, 0)
			retDir = FVector(0.f, -1.f, 0.f);
			break;
		case 7:
			// 315도: 오른쪽 아래
			retDir = FVector(.707f, -.707f, 0.f);

			break;
		default:
			// 예외 상황 (실제로는 여기로 진입하지 않음)
			retDir = GetVelocity().GetSafeNormal();
			break;
		}

		return retDir;
	}
	return FVector(1, 0, 0);

}

void ASOTestCharacter::OnRep_ConsumedFood()
{
	UE_LOG(LogTemp, Warning, TEXT("Consumed Food"));

}

void ASOTestCharacter::AddConsumedFood(int food)
{
	//서버일때만 처리
	if (HasAuthority())
	{
		ConsumedFood += food;
		CurRemainingFoodCnt += food;
		
		//추가해야할 몸통의 갯수
		int AddBodyCnt = CurRemainingFoodCnt / FoodCntToAddBody;
		if (AddBodyCnt >= 0)
		{
			//AddBodyCnt만큼 추가
			for(int i=0;i<AddBodyCnt;i++)
				AddBody();
			//추가하고 남은 값 갱신
			CurRemainingFoodCnt =AddBodyCnt % FoodCntToAddBody;
		}
	}
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

void ASOTestCharacter::MoveBodySegments(float DeltaTime)
{
	 
	//bodycomonents는 머리는 제외 
	//따라서 bodycomonentsloc[i]는 bodycomponent[i-1]의 location
	for (int i = 0; i < BodyComponents.Num(); i++)
	{
		FVector currentPos = BodyComponentsLoc[i+1];
		FVector TargetPos = BodyComponentsLoc[i];
		FVector direction = (TargetPos - currentPos).GetSafeNormal(); // 방향 계산
		FVector newLocation = BodyComponents[i]->GetComponentLocation() + direction * moveSpeed * DeltaTime; // 이동 계산
		BodyComponents[i]->SetWorldLocation(newLocation, true);
	}
}
//전체 몸 기록하는 함수 
//
void ASOTestCharacter::SetBodyLocation()
{
	BodyComponentsLoc[0] = HeadComponent->GetComponentLocation();

	for (int i = 0; i < BodyComponents.Num(); i++)
	{
		BodyComponentsLoc[i + 1] = BodyComponents[i]->GetComponentLocation();
	}
}

void ASOTestCharacter::CToSSetMaterial_Implementation(UMaterialInterface* NewMaterial)
{
	//서버는 메테리얼 정보 필요 x 클라에서 처리해야함
	ReplicatedMaterial = NewMaterial;
}

