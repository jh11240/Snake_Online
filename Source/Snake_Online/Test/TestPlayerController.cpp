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
#include "Subsystem/SOServerSubsystem.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Test/GameOverUserWidget.h"
#include "InputActionValue.h"



ATestPlayerController::ATestPlayerController()
{
    //// 블루프린트 클래스를 찾고 TSubclassOf에 할당
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Lobby/SOGameOver.SOGameOver_C'"));
    if (WidgetClassFinder.Succeeded())
    {
        GameOverWidget = WidgetClassFinder.Class;
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


void ATestPlayerController::OnPossess(APawn* pawn)
{
    Super::OnPossess(pawn);
#pragma region 무적시간 핸들링
    ASOTestCharacter* Snake = Cast<ASOTestCharacter>(pawn);
    if (Snake)
    {
        Snake->InitSnake();
                // 일정 시간 후 무적 해제
        FTimerHandle Handle;
        GetWorld()->GetTimerManager().SetTimer(Handle, [Snake]()
        {
            Snake->SetInvincible(false);
        }, Snake->GetInvincibleTime(), false);
    }
#pragma endregion 

#pragma region 플레이어 큐에 넣기
    if (UGameInstance* gameInstance = GetGameInstance())
	{
		USOServerSubsystem* ServerSystem = gameInstance->GetSubsystem<USOServerSubsystem>();
		if (ServerSystem)
		{
            ServerSystem->AddSnakePlayerToQueue(this);
		}
    }
#pragma endregion

#pragma region 서버단에서 캐릭터, 캐릭터무브먼트 캐싱
    CachedControlledChar = Cast<ACharacter>(pawn);
    if (!CachedControlledChar)
    {
        ensure(false);
        return;
    }
    //move speed 캐싱
    ASOTestCharacter* testCharacter = Cast<ASOTestCharacter>(pawn);
    moveSpeed = testCharacter->GetMoveSpeed();

    CachedCharMovement = CachedControlledChar->GetCharacterMovement();
    if (!CachedCharMovement)
    {
        ensure(false);
        return;
    }
#pragma endregion

    //스폰시 다른 방향으로 이동하도록 설정
    FVector startDir = CalStartDir();
    CToSMove(startDir, 1);

}

void ATestPlayerController::BeginPlay()
{
    Super::BeginPlay();
    if (IsLocalController()) {

        //입력 처리 설정
        UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
        if (Subsystem)
        {
            Subsystem->AddMappingContext(IMC_Move, 0);
        }

        //인풋 모드 게임 온리로 변경
        UWidgetBlueprintLibrary::SetInputMode_GameOnly(this, true);

        //커서 끄기
        bShowMouseCursor = false;
    }
}

void ATestPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    //enhancedInputComponent에 액션 바인딩
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
        CachedControlledChar = ControlledCharacter;
        CachedCharMovement = CachedControlledChar->GetCharacterMovement();
        UGameInstance* gameInstance = GetGameInstance();
        if (gameInstance)
        {
            USOServerSubsystem* subSystem = gameInstance->GetSubsystem<USOServerSubsystem>();
            if (subSystem)
            {
                int32 id = subSystem->GetClientID();
                UE_LOG(LogTemp, Display, TEXT("game level에서 호출 id : %d"),id);
                SetPlayerInfo(ControlledCharacter, id);

            }
            else
                check(false);
        }
        else
            check(false);

        //moveSpeed = ControlledCharacter->GetMoveSpeed();
        if (GameOverWidget && createdGameOverWidget == nullptr)
        {
            createdGameOverWidget = Cast<UUserWidget>(CreateWidget<UUserWidget>(GetWorld(), GameOverWidget));
            if (createdGameOverWidget)
            {
                createdGameOverWidget->AddToViewport(); // 화면에 위젯 표시
            }
            createdGameOverWidget->SetVisibility(ESlateVisibility::Hidden);
        }
        //// Character 초기화 로직
        //USOTESTUserWidget* tmpWidget = Cast<USOTESTUserWidget>(createdLobbyWidget);
        //tmpWidget->OnChangeMaterial.AddUniqueDynamic(ControlledCharacter, &ASOTestCharacter::SetSnakeMaterial);
    }
}

void ATestPlayerController::SetPlayerInfo(ASOTestCharacter* playerCharacter, int32 playerId)
{
    //서버에서 처리할 정보들 (현재는 material만)
    CToSSetPlayerInfo(playerCharacter, playerId);
}

void ATestPlayerController::GameOver()
{
    if (HasAuthority())
    {
        return;
    }
    createdGameOverWidget->SetVisibility(ESlateVisibility::Visible);
    bShowMouseCursor = true;
    SetInputMode(FInputModeUIOnly());

}

void ATestPlayerController::SetGameOverInfo(int32 place)
{
    UGameOverUserWidget* gameOverWidget = Cast<UGameOverUserWidget>(createdGameOverWidget);
    if (gameOverWidget)
        gameOverWidget->SetPlaceText(place);
    else
        check(false);
}




void ATestPlayerController::CToSMove_Implementation(const FVector& Direction, float Value)
{
    //일단 Value값 무시 -> velocity에 Value값으로 조절하려했는데 오차값때문에 혹시 처리방식 달라질까봐 무시
    FVector NewVelocity = Direction  * moveSpeed;
    CachedCharMovement->Velocity = NewVelocity;
}



void ATestPlayerController::CToSSetPlayerInfo_Implementation(ASOTestCharacter* character,int32 clientID)
{
    
        UGameInstance* gameInstance = GetGameInstance();
        if (gameInstance)
        {
            USOServerSubsystem* subSystem = gameInstance->GetSubsystem<USOServerSubsystem>();
            if (subSystem)
            {
                FPlayerSettings playerSetting = subSystem->GetPlayerSetting(clientID);
                UE_LOG(LogTemp, Display, TEXT("SErver에서 전달받은 clientId : %d"),clientID);
                character->ServerSetMaterial(playerSetting.PlayerMaterialIdx);
                character->SetNameWidget(playerSetting.PlayerName);
            }
        }
}

void ATestPlayerController::OnMove(const FInputActionValue& InputActionValue)
{

    const FVector2D ActionValue = InputActionValue.Get<FVector2D>();
    const FRotator Rotation = K2_GetActorRotation();
    const FRotator RotationYaw = FRotator(0.0, Rotation.Yaw, 0.0);
    const FVector ForwardVector = UKismetMathLibrary::GetForwardVector(RotationYaw);
    const FVector RightVector = UKismetMathLibrary::GetRightVector(RotationYaw);
    FVector MovementDirection = (ForwardVector * ActionValue.X) + (RightVector * ActionValue.Y);

    // 만약 MovementDirection이 0이 아니라면 정규화하여 단위 벡터로 만든 후, 입력 크기를 별도로 계산
    MovementDirection = MovementDirection.GetSafeNormal();
    if (MovementDirection.IsNearlyZero())
    {
        return; 
    }

    if (CachedCharMovement)
    {
        FVector curVelocity = CachedCharMovement->Velocity.GetSafeNormal();
        if (curVelocity.Equals(MovementDirection, KINDA_SMALL_NUMBER))
        {
            UE_LOG(LogTemp, Warning, TEXT("%s 이전 방향과 같은 방향임 rpc호출x"),ANSI_TO_TCHAR(__FUNCTION__));
            return;
        }
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

FVector ATestPlayerController::CalStartDir()
{
    USOServerSubsystem* serverSystem = GetGameInstance()->GetSubsystem<USOServerSubsystem>();
    int32 dirIdx = serverSystem->GetStartDirIdx();
    FVector startDir = FVector(dirX[dirIdx], dirY[dirIdx], 0);
    startDir.Normalize();
    return startDir;
}
