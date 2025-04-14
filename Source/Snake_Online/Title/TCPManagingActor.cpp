// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TCPManagingActor.h"

#include "Containers/StringConv.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "TitleHUD.h"
#include "UI/Title/TitleUserWidget.h"

// Networking 관련 헤더
#include "Utils/NetworkUtils.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"
// Sets default values
ATCPManagingActor::ATCPManagingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATCPManagingActor::BeginPlay()
{
	Super::BeginPlay();

    //런처에서 정상적으로 받아왔을 때
    if (GetPortFromLauncher())
    {
        //widget 접속 버튼 활성화시키기
        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            ATitleHUD* MyHUD = Cast<ATitleHUD>(PC->GetHUD());
            if (MyHUD)
            {
                MyHUD->OnWidgetAddedToViewport.AddDynamic(this, &ATCPManagingActor::ActivateLoginButton);
                UTitleUserWidget* UserWidget = MyHUD->GetTitleInfoWidget();
                if (UserWidget)
                {
                    UE_LOG(LogTemp, Display, TEXT("%s USERWIDGET 찾아옴!"), ANSI_TO_TCHAR(__FUNCTION__));

                    ActivateLoginButton(UserWidget);
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("%s USERWIDGET 못찾아옴"), ANSI_TO_TCHAR(__FUNCTION__));
                    check(false);
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("%s HUD 못찾아옴"), ANSI_TO_TCHAR(__FUNCTION__));
                check(false);
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%s PC 못찾아옴"),ANSI_TO_TCHAR(__FUNCTION__));
            check(false);
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("런처프로그램으로부터 포트 못받아와서 접속 불가!"));
        check(false);
        return;
    }


}

bool ATCPManagingActor::GetPortFromLauncher()
{

#pragma region 소켓 생성해서 받아오는 시퀀스
    // 1. 소켓 생성 (TCP 스트림 소켓)
    FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_CLIENT"), false);
    if (!Socket)
    {
        UE_LOG(LogTemp, Error, TEXT("소켓 생성 실패"));
        return false;
    }

    // 2. 연결할 서버 주소 설정 (테스트용 127.0.0.1:7777)
    FIPv4Address IP;
    FIPv4Address::Parse(SO::NetworkUtils::GetIP(), IP);

    TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    Addr->SetIp(IP.Value);
    int32 port = FCString::Atoi(*(SO::NetworkUtils::GetPort()));
    Addr->SetPort(port);

    // 3. 서버에 연결
    bool bConnected = Socket->Connect(*Addr);
    if (!bConnected)
    {
        UE_LOG(LogTemp, Error, TEXT("서버에 연결 실패: %s"), *Addr->ToString(true));
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        return false;
    }
    //4. requset msg 전송
    const uint8* DataBuffer = reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*serverRequestMsg));
    int32 DataSize = FCStringAnsi::Strlen(reinterpret_cast<const char*>(DataBuffer));
    int32 BytesSent = 0;

    bool bSent = Socket->Send(DataBuffer, DataSize, BytesSent);
    if (!bSent)
    {
        UE_LOG(LogTemp, Error, TEXT("메시지 전송 실패"));
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        return false;
    }
    else
    {
        UE_LOG(LogTemp, Log, TEXT("Sent: %s (%d bytes)"), *serverRequestMsg, BytesSent);
    }

    // 5. 수신을 위해 소켓을 블로킹 모드로 전환 (간단한 예제용)
    Socket->SetNonBlocking(false);

    // 6. 에코된 응답 수신
    uint8 Buffer[1024];
    int32 BytesRead = 0;
    bool bReceived = Socket->Recv(Buffer, sizeof(Buffer) - 1, BytesRead);
    if (bReceived && BytesRead > 0)
    {
        // 수신된 바이트 뒤에 NULL 추가 (문자열 완전화를 위하여)
        Buffer[BytesRead] = '\0';
        int32 ReceivedPort = *reinterpret_cast<int32*>(Buffer);
        SO::NetworkUtils::SetPort(FString::FromInt(ReceivedPort));
        UE_LOG(LogTemp, Log, TEXT("Received: %d (%d bytes)"), ReceivedPort, BytesRead);
    }
    //메시지 수신 실패시 false로 리턴
    else
    {
        UE_LOG(LogTemp, Error, TEXT("메시지 수신 실패"));
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        return false;
    }

    // 7. 연결 종료 및 소켓 해제
    Socket->Close();
    ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
#pragma endregion
    return true;
}

void ATCPManagingActor::ActivateLoginButton(UUserWidget* inWidget)
{
    UTitleUserWidget* widget = Cast<UTitleUserWidget>(inWidget);
    widget->SetLoginBtnActivate(true);
}

// Called every frame
void ATCPManagingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

