#pragma once

#include "CoreMinimal.h"

namespace SO
{
namespace CollisionProfileName
{
	static inline FName Food = TEXT("Food");		// 플레이어, 적, Pawn을 감지하는 트리거
	static inline FName Head = TEXT("Head");	// 플레이어를 감지하는 트리거
	static inline FName Wall = TEXT("Wall");
	static inline FName Body = TEXT("Body");
}
namespace TagName
{
	static inline FName Food = TEXT("Food");		// 플레이어, 적, Pawn을 감지하는 트리거
	static inline FName Head = TEXT("Head");	// 플레이어를 감지하는 트리거
	static inline FName Wall = TEXT("Wall");
	static inline FName Body = TEXT("Body");
}

	class SNAKE_ONLINE_API SOUtils
	{
	public:
		

	private:
		
	};
}