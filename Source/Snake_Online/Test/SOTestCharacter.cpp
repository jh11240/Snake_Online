// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestCharacter.h"

// Sets default values
ASOTestCharacter::ASOTestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	{
			static ConstructorHelpers::FObjectFinder<UDataTable> Asset(TEXT("/Script/Engine.DataTable'/Game/Test/Datatable/Snake.Snake'"));
			check(Asset.Object);
			UDataTable* SnakeDataTable = Asset.Object;
			SnakeDataTableRowHandle.DataTable = SnakeDataTable;
			SnakeDataTableRowHandle.RowName = "Green";

	}

	bReplicates = true;
	NetCullDistanceSquared = 100000000.0f;
	bAlwaysRelevant = true;
	bUseControllerRotationYaw = false;

	//GetMesh()->SetSkeletalMesh()

	SnakeStaticMeshCompoenent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SnakeStaticMeshCompoenent->SetupAttachment(RootComponent);

	//GetCapsuleComponent()->SetCollisionProfileName(CollisionProfileName::Player);
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
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

}

void ASOTestCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

}

// Called when the game starts or when spawned
void ASOTestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		OnRep_UpdatePawnDataTable();
	}
	// 클라중 내 클라만(ROLE_AutonomousProxy)
	/*else if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		
	}*/
}

// Called every frame
//void ASOTestCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

// Called to bind functionality to input
void ASOTestCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ASOTestCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, SnakeDataTableRowHandle);
}

void ASOTestCharacter::OnRep_UpdatePawnDataTable()
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

	}

	{
	
		SnakeStaticMeshCompoenent->SetStaticMesh(Data->StaticMesh);
		SnakeStaticMeshCompoenent->SetIsReplicated(true);

		// ACharacter::PostInitializeComponents() 시점에 초기에 설정된 Mesh의 RelativeLocation, RelativeRotation을 받아와서
		// CharacterMovementComponent에서 사용하고 있음.
		// 우리는 데이터를 받아와서 적용하고 있기 때문에 한번 더 설정 필요
		// * 이부분을 뺴면 내 Character는 정상적으로 보이지만, 다른 플레이어의 Character는 이동되기전 위치 정보로 처리됨
		// 이 데이터를 통해서 클라 UCharacterMovementComponent::SmoothClientPosition_UpdateVisuals() 에서 Mesh의 위치를 바꾸고 있음.
		// (Mesh->SetRelativeLocationAndRotation(NewRelTranslation, NewRelRotation, false, nullptr, GetTeleportType());)
		CacheInitialMeshOffset(SnakeData->MeshTransform.GetTranslation(), SnakeData->MeshTransform.GetRotation().Rotator());
	}
}

