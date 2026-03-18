// ynGameStateBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ynGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class YN_NINE_TASK_API AynGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));
};
