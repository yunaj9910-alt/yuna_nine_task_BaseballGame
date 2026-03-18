// ynPlayerState.cpp


#include "Player/ynPlayerState.h"
#include "Net/UnrealNetwork.h"


AynPlayerState::AynPlayerState():PlayerNameString(TEXT("None")),CurrentGuessCount(0),MaxGuessCount(3)
{
	bReplicates = true;
}

void AynPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerNameString);
	DOREPLIFETIME(ThisClass, CurrentGuessCount);
	DOREPLIFETIME(ThisClass, MaxGuessCount);

}

FString AynPlayerState::GetPlayerInfoString()
{
	//FString PlayerInfoString = PlayerNameString + TEXT("(") + FString::FromInt(MaxGuessCount) + TEXT(")");
	//return PlayerInfoString;
	return FString::Printf(TEXT("%s [%d / %d]"), *PlayerNameString, CurrentGuessCount+1, MaxGuessCount);
}