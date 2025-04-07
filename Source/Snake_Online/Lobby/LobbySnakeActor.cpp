// Fill out your copyright notice in the Description page of Project Settings.


#include "Lobby/LobbySnakeActor.h"
#include "Test/SnakeData.h"
#include "UI/PlayerNameUserWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
ALobbySnakeActor::ALobbySnakeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
#pragma region material 할당
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
		MaterialSize = Materials.Num();

	}
#pragma endregion
#pragma region staticmesh 생성
	{
		HeadComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

		HeadComponent->SetGenerateOverlapEvents(true);
		HeadComponent->SetEnableGravity(false);
		static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
		if (MeshAsset.Succeeded())
		{
			SphereMesh = MeshAsset.Object;
			HeadComponent->SetStaticMesh(SphereMesh);
		}
		RootComponent= HeadComponent;
		HeadComponent->SetRelativeLocation(FVector(0, 0, -RotateRadius));
	}
#pragma endregion
#pragma region NameUI 세팅
	{
		// HP Bar
		{///Script/UMGEditor.WidgetBlueprint'/Game/UI/Lobby/SOPlayerName.SOPlayerName'
			ConstructorHelpers::FClassFinder<UPlayerNameUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Lobby/SOPlayerName.SOPlayerName_C'"));
			NameTextComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("NameWidgetComponent"));
			NameTextComponent->SetupAttachment(HeadComponent);
			NameTextComponent->SetRelativeLocation(FVector(0, 0, 150.0));
			NameTextComponent->SetDrawSize(FVector2D(256.3, 17.0));
			NameTextComponent->SetWidgetSpace(EWidgetSpace::Screen);
			NameTextComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			check(WidgetClass.Class);
			NameTextComponent->SetWidgetClass(WidgetClass.Class);
		}
	}
#pragma endregion
}

// Called when the game starts or when spawned
void ALobbySnakeActor::BeginPlay()
{
	Super::BeginPlay();
	FVector headWorldLoc = FVector(0,0,-500);
	BodyComponentsLoc.Add(headWorldLoc);
	for (int i = 0; i < 6; i++)
		AddBody();
	if (BodyDiameter != 0)
		BodyMoveRefreshRate = BodyDiameter / moveSpeed;
	else
		BodyMoveRefreshRate = 1.f;
}

void ALobbySnakeActor::SetSnakeMaterial(uint32 materialIdx)
{
	UMaterialInterface* materialToSet = Materials[materialIdx];
	HeadComponent->SetMaterial(0, materialToSet);

	for (UStaticMeshComponent* elem : BodyComponents)
	{
		elem->SetMaterial(0, materialToSet);
	}
}

void ALobbySnakeActor::SetNextSnakeMaterial()
{
	curIdx = (curIdx + 1) % MaterialSize;
	SetSnakeMaterial(curIdx);
}

void ALobbySnakeActor::SetPrevSnakeMaterial()
{
	curIdx = curIdx ==0?  MaterialSize-1 : (curIdx-1)%MaterialSize;
	SetSnakeMaterial(curIdx);
}

void ALobbySnakeActor::SetNameText(FText InName)
{
	UPlayerNameUserWidget* playerNameWidget = Cast<UPlayerNameUserWidget>(NameTextComponent->GetWidget());
	if (playerNameWidget)
	{
		playerNameWidget->SetNameText(InName);
	}
}

void ALobbySnakeActor::SetBodyLocation()
{
	BodyComponentsLoc[0] = HeadComponent->GetComponentLocation();

	for (int i = 0; i < BodyComponents.Num(); i++)
	{
		BodyComponentsLoc[i + 1] = BodyComponents[i]->GetComponentLocation();
	}
}

void ALobbySnakeActor::MoveBodySegments(float DeltaTime)
{
	for (int i = 0; i < BodyComponents.Num(); i++)
	{
		FVector currentPos = BodyComponentsLoc[i + 1];
		FVector TargetPos = BodyComponentsLoc[i];
		FVector direction = (TargetPos - currentPos).GetSafeNormal(); // 방향 계산
		FVector newLocation = BodyComponents[i]->GetComponentLocation() + direction * moveSpeed * DeltaTime; // 이동 계산
		BodyComponents[i]->SetWorldLocation(newLocation, true);
	}
}

// Called every frame
void ALobbySnakeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
#pragma region 머리 먼저 돌리기
	// 회전 속도 (라디안 단위, 예: 90도/s는 약 1.5708 rad/s)
	float AngularSpeed = FMath::DegreesToRadians(RotationSpeed);
	// RotationSpeed가 초당 회전할 각도 (deg/s)라면,
	RotateAngle += RotationSpeed * DeltaTime;
	if (RotateAngle >= 360.0f)
	{
		RotateAngle -= 360.0f;
	}
	float Angle = FMath::DegreesToRadians(RotateAngle);

	// 원의 중심(예제에서는 현재 액터의 X 좌표는 유지하고 Y-Z 좌표 기준 원의 중심)
	// 필요에 따라 Center를 별도로 지정할 수 있습니다.
	FVector Center = FVector(GetActorLocation().X, 0.0f, 0.0f);

	// Y-Z 평면에서의 원형 경로 계산
	float NewY = Center.Y + RotateRadius * FMath::Cos(Angle);
	float NewZ = Center.Z + RotateRadius * FMath::Sin(Angle);

	FVector NewLocation = FVector(Center.X, NewY, NewZ);
	SetActorLocation(NewLocation);
#pragma endregion

	//몸통 따라가게 하기
	MoveBodySegments(DeltaTime);
	if (curSec < BodyMoveRefreshRate) {
		curSec += DeltaTime;
		return;
	}
	SetBodyLocation();
	curSec = 0;
}

void ALobbySnakeActor::AddBody()
{
	// 새 Body Segment 생성
	UStaticMeshComponent* NewBodySegment = NewObject<UStaticMeshComponent>(this);

	// 초기 설정
	NewBodySegment->SetGenerateOverlapEvents(true); // Overlap 이벤트 활성화
	NewBodySegment->SetStaticMesh(SphereMesh); // 사용할 Mesh를 설정
	NewBodySegment->RegisterComponent(); // 컴포넌트 등록

	// BodyComponents 배열에 추가
	BodyComponents.Add(NewBodySegment);
	NewBodySegment->SetUsingAbsoluteLocation(true);
	NewBodySegment->SetUsingAbsoluteRotation(true);

#pragma region 부모-자식 관계 설정 (이미 등록된 상태이므로 AttachToComponent 사용)

	NewBodySegment->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

#pragma endregion

#pragma region 새 노드의 위치 설정
	FVector SpawnLocation = FVector::ZeroVector;
	FVector CurrentDirection = FVector(0,1,0);

	if (BodyDiameter == 0)
	{
		BodyDiameter = SphereMesh->GetBounds().BoxExtent.Y * 2.0f;
	}
	if (BodyComponents.Num() > 1)
	{
		UStaticMeshComponent* PreviousSegment = BodyComponents[BodyComponents.Num() - 2];
		FVector PreviousLocation = PreviousSegment->GetComponentLocation();
		SpawnLocation = PreviousLocation - CurrentDirection * BodyDiameter; // -Y축으로 이동

	}
	else
	{
		SpawnLocation = HeadComponent->GetComponentLocation() - CurrentDirection * BodyDiameter;
	}

	NewBodySegment->SetWorldLocation(SpawnLocation);

	FVector bodyWorldLoc = NewBodySegment->GetComponentLocation();
	BodyComponentsLoc.Add(bodyWorldLoc);
#pragma endregion
}

