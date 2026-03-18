// ynGameStateBase.cpp

#include "Game/ynGameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ynPlayerController.h"

void AynGameStateBase::MulticastRPCBroadcastLoginMessage_Implementation(const FString& InNameString)
{
	if (HasAuthority() == false)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (IsValid(PC) == true)
		{
			AynPlayerController* CXPC = Cast<AynPlayerController>(PC);
			if (IsValid(CXPC) == true)
			{
				FString NotificationString = InNameString + TEXT(" has joined the game.");
				CXPC->PrintChatMessageString(NotificationString);
			}
		}
	}
}
