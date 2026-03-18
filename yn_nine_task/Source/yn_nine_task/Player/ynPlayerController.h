// ynPlayerController.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ynPlayerController.generated.h"


class UynChatInput;
class UUserWidget;

UCLASS()
class YN_NINE_TASK_API AynPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AynPlayerController();

	virtual void BeginPlay() override;

	void SetChatMessageString(const FString& InChatMessageString);

	void PrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Client, Reliable)
	void ClientRPCPrintChatMessageString(const FString& InChatMessageString);

	UFUNCTION(Server, Reliable)
	void ServerRPCPrintChatMessageString(const FString& InChatMessageString);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UynChatInput>ChatInputWidgetClass;

	UPROPERTY()
	TObjectPtr<UynChatInput> ChatInputWidgetInstance;

	FString ChatMessageString;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> NotificationTextWidgetClass;
	UPROPERTY()
	TObjectPtr<UUserWidget> NotificationTextWidgetInstance;

public:
	UPROPERTY(Replicated, BlueprintReadOnly)
	FText NotificationText;
};
