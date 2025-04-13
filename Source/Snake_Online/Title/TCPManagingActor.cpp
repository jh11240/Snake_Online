// Fill out your copyright notice in the Description page of Project Settings.


#include "Title/TCPManagingActor.h"

// Networking 관련 헤더
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "IPAddress.h"
#include "Interfaces/IPv4/IPv4Address.h"

#include "Containers/StringConv.h"
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
	
	// 1. 소켓 생성 (TCP 스트림 소켓)
    FSocket* Socket = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("TCP_CLIENT"), false);
    if (!Socket)
    {
        UE_LOG(LogTemp, Error, TEXT("소켓 생성 실패"));
        return;
    }

    // 2. 연결할 서버 주소 설정 (127.0.0.1:7777)
    FIPv4Address IP;
    FIPv4Address::Parse(TEXT("127.0.0.1"), IP);

    TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
    Addr->SetIp(IP.Value);
    Addr->SetPort(7777);

    // 3. 서버에 연결
    bool bConnected = Socket->Connect(*Addr);
    if (!bConnected)
    {
        UE_LOG(LogTemp, Error, TEXT("서버에 연결 실패: %s"), *Addr->ToString(true));
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        return;
    }

    // 4. "hello" 메시지 전송 (UTF8 인코딩)
    //FString Message = TEXT("hello");
    ////StringCast<ANSICHAR> UTF8String(*Message);  // ANSICHAR는 UTF-8 문자 타입

    //UTF8CHAR* UTF8Message = StringCast<UTF8CHAR>(*Message);
    //    //UTF8String.Get();
    //int32 DataSize = UTF8String.Length();
    // 기존의 자료형을 const uint8* 형태로 강제 변환
    const uint8* DataBuffer = reinterpret_cast<const uint8*>(TCHAR_TO_UTF8(*serverRequestMsg));
    int32 DataSize = FCStringAnsi::Strlen(reinterpret_cast<const char*>(DataBuffer));
    // 또는, 이미 UTF-8인 데이터를 다루고 있다면 적절한 방법으로 DataSize를 설정
    int32 BytesSent = 0;
    
    bool bSent = Socket->Send(DataBuffer, DataSize, BytesSent);
    if (!bSent)
    {
        UE_LOG(LogTemp, Error, TEXT("메시지 전송 실패"));
        Socket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
        return;
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
        UE_LOG(LogTemp, Log, TEXT("Received: %d (%d bytes)"), ReceivedPort, BytesRead);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("메시지 수신 실패"));
    }

    // 7. 연결 종료 및 소켓 해제
    Socket->Close();
    ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Socket);
}

// Called every frame
void ATCPManagingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

