#pragma once

#include "CoreMinimal.h"

namespace SO
{
	class /*SNAKE_ONLINE_API*/ NetworkUtils
	{
	public:


		static FString GetServerURL() { return FURL; };
		static FString GetGameMapURL() { return GameMap; };
		static FString GetTitleURL() { return TitleMap; };
		static FString GetIP() { return IP; }
		static FString GetPort() { return Port; }
		
		static void SetPort(FString inPort) 
		{ 
			Port = inPort;
			//FURL 변경된 포트로 재구성
			FURL = IP + TEXT(":") + Port;
		}

	private:
		//static inline FString FURL = TEXT("13.124.239.135:7777");
		//게임런처 프로그램에 진입 url

		//통 ip
		static inline FString IP = TEXT("52.78.120.162");
		static inline FString Port = TEXT("7777");

		static inline FString FURL = IP + TEXT(":") + Port;
		static inline FString GameMap = TEXT("/Game/Test/Test");
		static inline FString TitleMap = TEXT("/Game/Level/Title");

	};
}