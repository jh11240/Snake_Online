#pragma once

#include "CoreMinimal.h"

namespace SO
{
	class SNAKE_ONLINE_API NetworkUtils
	{
	public:
		static FString GetServerURL() { return FURL; };

	private:
		static inline FString FURL = TEXT("127.0.0.1:7777");
	};
}