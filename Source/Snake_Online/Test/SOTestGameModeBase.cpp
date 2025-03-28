// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/SOTestGameModeBase.h"
#include "Test/TestPlayerController.h"
#include "Test/SOTestCharacter.h"
#include "Test/SnakeFood.h"

ASOTestGameModeBase::ASOTestGameModeBase()
{
	PlayerControllerClass = ATestPlayerController::StaticClass();
	DefaultPawnClass = ASOTestCharacter::StaticClass();
}

void ASOTestGameModeBase::BeginPlay()
{
	Super::BeginPlay();
    // 게임 시작 후 일정 시간 간격으로 먹이를 스폰하도록 타이머 설정 예시
    GetWorldTimerManager().SetTimer(FoodSpawnTimerHandle, this, &ASOTestGameModeBase::SpawnFood, 5.0f, true);
    
}

void ASOTestGameModeBase::SpawnFood()
{

        // 서버 권한인지 검사
        if (!HasAuthority())
        {
            return;
        }

        // 먹이 스폰 위치를 랜덤으로 계산
        FVector SpawnLocation = GetRandomLocation()/* 랜덤 위치 계산 로직 */;
        FRotator SpawnRotation = FRotator::ZeroRotator;

        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;  // 필요 시

        // 먹이 액터 생성 (스폰)
        ASnakeFood* NewFood = GetWorld()->SpawnActor<ASnakeFood>(ASnakeFood::StaticClass(), SpawnLocation, SpawnRotation, SpawnParams);
        if (NewFood)
        {
            // 생성 후
            NewFood->SetFoodScore(1);
        }
    
}

FVector ASOTestGameModeBase::GetRandomLocation()
{
    FBox SpawnBox(MinBound, MaxBound);

    // SpawnBox의 내부에서 랜덤한 점을 반환
    return FMath::RandPointInBox(SpawnBox);

}
