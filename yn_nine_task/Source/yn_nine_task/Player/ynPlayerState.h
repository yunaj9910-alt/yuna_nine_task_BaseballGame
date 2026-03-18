// ynPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ynPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class YN_NINE_TASK_API AynPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AynPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	FString GetPlayerInfoString();
public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;
};
