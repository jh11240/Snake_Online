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
		
	private:
		static inline FString FURL = TEXT("127.0.0.1:7777");
		static inline FString GameMap = TEXT("/Game/Test/Test");
		static inline FString TitleMap = TEXT("/Game/Level/Title");

	};
}