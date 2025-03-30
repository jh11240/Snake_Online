// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestPlayerController.h"
#include "Test/SOTestCharacter.h"
#include "UI/Test/SOTESTUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Kismet/KismetMathLibrary.h"

#include "InputActionValue.h"



ATestPlayerController::ATestPlayerController()
{
    //// 블루프린트 클래스를 찾고 TSubclassOf에 할당
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/SOLogin.SOLogin_C'"));
    if (WidgetClassFinder.Succeeded())
    {
        LobbyWidget = WidgetClassFinder.Class;
        UE_LOG(LogTemp, Warning, TEXT("UserWidgetClass successfully assigned."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to find UserWidgetClass!"));
    }

    {
        static ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
        { TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/MoveContext.MoveContext'") };
        check(Asset.Object);

        IMC_Move= Asset.Object;
    }
}

const UInputAction* GetInputActionFromName(UInputMappingContext* IMC, const FName& InName)
{
    const TArray<FEnhancedActionKeyMapping>& Mappings = IMC->GetMappings();
    for (auto& It : Mappings)
    {
        if (It.Action->GetFName() == InName)
        {
            return It.Action.Get();
        }
    }

    return nullptr;
}


void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();

    UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    if (Subsystem)
    {
        Subsystem->AddMappingContext(IMC_Move, 0);
    }

    //TODO: Map처리
    if (IsLocalController()) // 로컬 플레이어 컨트롤러인지 확인
    {
        //잘됨
        //// 현재 레벨 경로를 가져와 비교
        //FString CurrentLevelName = GetWorld()->GetOutermost()->GetName();
        //FString TargetLevelName = TEXT("/Game/Test/Test");

        //if (CurrentLevelName.Equals(TargetLevelName))
        //{
        //    return;
        //// 추가 로직을 여기에 작성
        //}

        //if (LobbyWidget && createdLobbyWidget == nullptr) // UMG 블루프린트 클래스 확인
        //{
        //    createdLobbyWidget =Cast<UUserWidget>( CreateWidget<UUserWidget>(GetWorld(), LobbyWidget));
        //    if (createdLobbyWidget)
        //    {
        //        createdLobbyWidget->AddToViewport(); // 화면에 위젯 표시
        //    }

        //}
        bShowMouseCursor = true;
    }
}

void ATestPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
    ensure(EnhancedInputComponent);

    if (const UInputAction* MoveAction = GetInputActionFromName(IMC_Move, TEXT("MoveAction")))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
    }
}

void ATestPlayerController::OnRep_Pawn()
{
    Super::OnRep_Pawn();

    ASOTestCharacter* ControlledCharacter = Cast<ASOTestCharacter>(GetPawn());
    if (ControlledCharacter)
    {
        if (LobbyWidget && createdLobbyWidget == nullptr)
        {
            createdLobbyWidget = Cast<UUserWidget>(CreateWidget<UUserWidget>(GetWorld(), LobbyWidget));
            if (createdLobbyWidget)
            {
                createdLobbyWidget->AddToViewport(); // 화면에 위젯 표시
            }
        }
        // Character 초기화 로직
        USOTESTUserWidget* tmpWidget = Cast<USOTESTUserWidget>(createdLobbyWidget);
        tmpWidget->OnChangeMaterial.AddUniqueDynamic(ControlledCharacter, &ASOTestCharacter::SetSnakeMaterial);
    }
}

//TODO : 키입력때 호출시 Packet 너무 많이 날려서 무시하는듯 -> 일단 하지말자
void ATestPlayerController::CToSMove_Implementation(const FVector& Direction, float Value)
{
    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) {
        ensure(false);
        return;
    }
    ACharacter* ControlledChar = Cast<ACharacter>(ControlledPawn);
    if (!ControlledChar)
    {
        ensure(false);
        return;
    }
    UCharacterMovementComponent* CharMovement = ControlledChar->GetCharacterMovement();
    if (!CharMovement)
    {
        ensure(false);
        return;
    }
    FVector NewVelocity = Direction * Value * moveSpeed;
    CharMovement->Velocity = NewVelocity;
}



void ATestPlayerController::OnMove(const FInputActionValue& InputActionValue)
{
    //죽었을 때 처리
    //if (StatusComponent && !StatusComponent->CanMove()) { return; }

    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = K2_GetActorRotation();
    const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
    const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
    const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) {
        ensure(false);
        return;
    }

    FVector MovementDirection = (ForwardVector * ActionValue.X) + (RightVector * ActionValue.Y);

    // 만약 MovementDirection이 0이 아니라면 정규화하여 단위 벡터로 만든 후, 입력 크기를 별도로 계산
    float InputMagnitude = MovementDirection.Size();
    if (InputMagnitude > KINDA_SMALL_NUMBER)
    {
        MovementDirection /= InputMagnitude;
    }
    else
    {
        MovementDirection = FVector::ZeroVector;
    }

    CToSMove(MovementDirection, 1.f);

    //원래 구현
    //if (ACharacter* ControlledChar = Cast<ACharacter>(ControlledPawn))
    //{
    //    if (UCharacterMovementComponent* CharMovement = ControlledChar->GetCharacterMovement())
    //    {
    //        CharMovement->StopMovementImmediately();
    //    }
    //}
    //ControlledPawn->AddMovementInput(ForwardVector, ActionValue.X * moveSpeed);
    //ControlledPawn->AddMovementInput(RightVector, ActionValue.Y * moveSpeed);
}
