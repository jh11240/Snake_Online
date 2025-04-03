// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestGameModeBase.h"
#include "Test/TestPlayerController.h"
#include "Test/SOTestCharacter.h"
#include "Test/SnakeFood.h"

ASOTestGameModeBase::ASOTestGameModeBase()
{
    PrimaryActorTick.bCanEverTick = true;

	PlayerControllerClass = ATestPlayerController::StaticClass();
	DefaultPawnClass = ASOTestCharacter::StaticClass();
}

void ASOTestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
    if (HasAuthority()) {
        // 게임 시작 후 일정 시간 간격으로 먹이를 스폰하도록 타이머 설정 예시
     //   GetWorldTimerManager().SetTimer(FoodSpawnTimerHandle, this, &ASOTestGameModeBase::SpawnFood, 5.0f, true);
    }
}

void ASOTestGameModeBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (HasAuthority()) 
    {
        if (curSec < spawnSec)
        {
            curSec += DeltaTime;
            return;
        }
        curSec = 0;
        SpawnFood();
    }
}

void ASOTestGameModeBase::SpawnFood()
{
    UE_LOG(LogTemp, Display, TEXT("음식 스폰 시작!"));
    // 서버 권한인지 검사
    if (HasAuthority())
    {
        // 먹이 스폰 위치를 랜덤으로 계산
        FVector SpawnLocation = GetRandomLocation();

        FRotator SpawnRotation = FRotator::ZeroRotator;
        UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f"), SpawnLocation.X, SpawnLocation.Y, SpawnLocation.Z);
        // 먹이 액터 생성 (스폰)
        ASnakeFood* NewFood = GetWorld()->SpawnActor<ASnakeFood>(ASnakeFood::StaticClass(), SpawnLocation, SpawnRotation);
        if (NewFood)
        {
            // 생성 후
            NewFood->SetFoodScore(1);
        }
    }
}

FVector ASOTestGameModeBase::GetRandomLocation()
{
    FBox SpawnBox(MinBound, MaxBound);

    // SpawnBox의 내부에서 랜덤한 점을 반환
    return FMath::RandPointInBox(SpawnBox);

}


