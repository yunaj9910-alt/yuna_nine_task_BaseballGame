//ynPlayerController.cpp


#include "Player/ynPlayerController.h"
#include "UI/ynChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "yn_nine_task.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ynGameModeBase.h"
#include "ynPlayerState.h"
#include "Net/UnrealNetwork.h"


AynPlayerController::AynPlayerController()
{
	bReplicates = true;
}

void AynPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);
	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UynChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}	

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AynPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	//PrintChatMessageString(ChatMessageString);
	if (IsLocalController() == true)
	{
		//ServerRPCPrintChatMessageString(InChatMessageString);
		AynPlayerState* CXPS = GetPlayerState<AynPlayerState>();
		if (IsValid(CXPS) == true)
		{
			//FString CombinedMessageString = CXPS->PlayerNameString + TEXT(": ") + InChatMessageString;
			FString CombinedMessageString = CXPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;
			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AynPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	//UKismetSystemLibrary::PrintString(this, ChatMessageString, true, true, FLinearColor::Red, 5.0f);
	FString NetModeString = yn_nine_taskFunctionLibrary::GetNetModeString(this);
	FString CombinedMessageString = FString::Printf(TEXT("%s: %s"), *NetModeString, *InChatMessageString);
	yn_nine_taskFunctionLibrary::MyprintString(this, CombinedMessageString, 10.f);
}

void AynPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NotificationText);
}

void AynPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AynPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	/*for (TActorIterator<AynPlayerController> It(GetWorld()); It; ++It)
	{
		AynPlayerController* ynPlayerController = *It;
		if (IsValid(ynPlayerController) == true)
		{
			ynPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
		}
	}*/

	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AynGameModeBase* CXGM = Cast<AynGameModeBase>(GM);
		if (IsValid(CXGM) == true)
		{
			CXGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}

