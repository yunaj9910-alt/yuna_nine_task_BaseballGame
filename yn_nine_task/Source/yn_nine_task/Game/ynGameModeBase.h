// ynGameModeBase.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ynGameModeBase.generated.h"

class AynPlayerController;
/**
 * 
 */
UCLASS()
class YN_NINE_TASK_API AynGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer)override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(AynPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(AynPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(AynPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AynPlayerController>>AllPlayerControllers;

	FTimerHandle ResetTimerHandle;
};
