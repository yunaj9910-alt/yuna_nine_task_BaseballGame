//ynGameNodeBase.cpp


#include "Game/ynGameModeBase.h"
#include "ynGameStateBase.h"
#include "Player/ynPlayerController.h"
#include "EngineUtils.h"
#include "Player/ynPlayerState.h"

void AynGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	/*AynGameStateBase* ynGameStateBase = GetGameState<AynGameStateBase>();
	if (IsValid(ynGameStateBase) == true)
	{
		ynGameStateBase->MulticastRPCBroadcastLoginMessage(TEXT("XXXXXXX"));
	}

	AynPlayerController* ynPlayerController = Cast<AynPlayerController>(NewPlayer);
	if (IsValid(ynPlayerController) == true)
	{
		AllPlayerControllers.Add(ynPlayerController);
	}*/

	AynPlayerController* ynPlayerController = Cast<AynPlayerController>(NewPlayer);
	if (IsValid(ynPlayerController) == true)
	{
		ynPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));
		AllPlayerControllers.Add(ynPlayerController);
		AynPlayerState* CXPS = ynPlayerController->GetPlayerState<AynPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}
		AynGameStateBase* ynGameStateBase =  GetGameState<AynGameStateBase>();
		if (IsValid(ynGameStateBase) == true)
		{
			ynGameStateBase->MulticastRPCBroadcastLoginMessage(CXPS->PlayerNameString);
		}
	}
}

FString AynGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)  // ← < 를 <= 로 바꿨어요. 이제 1~9 전부 들어가요
	{
		Numbers.Add(i);
	}

	// 섞기
	for (int32 i = Numbers.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		Numbers.Swap(i, j);
	}

	// 앞에서 3개만 뽑기
	FString Result = TEXT("");
	for (int32 i = 0; i < 3; ++i)
	{
		Result.Append(FString::FromInt(Numbers[i]));
	}
	UE_LOG(LogTemp, Error, TEXT("%s"), *Result);
	return Result;
}

bool AynGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {
		if (InNumberString.Len() != 3)
		{
			break;
		}
		bool bIsUnique = true;
		TSet<TCHAR>UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}
			UniqueDigits.Add(C);
		}
		if (bIsUnique == false)
		{
			break;
		}
		bCanPlay = true;
	} while (false);
	return bCanPlay;
}

FString AynGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AynGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void AynGameModeBase::PrintChatMessageString(AynPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	/*FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{

		IncreaseGuessCount(InChattingPlayerController);
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		for (TActorIterator<AynPlayerController> It(GetWorld()); It; ++It)
		{
			AynPlayerController* ynPlayerController = *It;
			if (IsValid(ynPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				ynPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

			}
		}

		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{*/
	/*for (TActorIterator<AynPlayerController> It(GetWorld()); It; ++It)
	{
		AynPlayerController* ynPlayerController = *It;
		if (IsValid(ynPlayerController) == true)
	{
		ynPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
	}
	}*/
		/*InChattingPlayerController->ClientRPCPrintChatMessageString(
			TEXT("Invalid input. Please enter 3 unique digits (1-9). No attempt consumed."));
	}*/
	
	AynPlayerState* PS = InChattingPlayerController->GetPlayerState<AynPlayerState>();
	if (IsValid(PS) == true)
	{
		if (PS->CurrentGuessCount >= PS->MaxGuessCount)
		{
			InChattingPlayerController->ClientRPCPrintChatMessageString(
				TEXT("No attempts remaining."));
			return;
		}
	}

	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);

	
	bool bValidFormat = false;
	if (Index > 0)
	{
	
		TCHAR CharBeforeGuess = InChatMessageString[Index - 1];
		if (CharBeforeGuess == ' ')
		{
			bValidFormat = true;
		}
	}

	if (bValidFormat == true && IsGuessNumberString(GuessNumberString) == true)
	{
		
		IncreaseGuessCount(InChattingPlayerController);
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		
		for (TActorIterator<AynPlayerController> It(GetWorld()); It; ++It)
		{
			AynPlayerController* ynPlayerController = *It;
			if (IsValid(ynPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				ynPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);
			}
		}

		
		int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
		JudgeGame(InChattingPlayerController, StrikeCount);
	}
	else
	{
		
		InChattingPlayerController->ClientRPCPrintChatMessageString(
			TEXT("Invalid input. Please enter 3 unique digits (1-9). No attempt consumed."));
	}
}

void AynGameModeBase::IncreaseGuessCount(AynPlayerController* InChattingPlayerController)
{
	AynPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AynPlayerState>();
	if (IsValid(CXPS) == true)
	{
		CXPS->CurrentGuessCount++;
	}
}

void AynGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& ynPlayerController : AllPlayerControllers)
	{
		AynPlayerState* CXPS = ynPlayerController->GetPlayerState<AynPlayerState>();
		if (IsValid(CXPS) == true)
		{
			CXPS->CurrentGuessCount = 0;
		}
	}
}

//void AynGameModeBase::JudgeGame(AynPlayerController* InChattingPlayerController, int InStrikeCount)
//{
//	if (3 == InStrikeCount)
//	{
//		AynPlayerState* CXPS = InChattingPlayerController->GetPlayerState<AynPlayerState>();
//		for (const auto& ynPlayerController : AllPlayerControllers)
//		{
//			if (IsValid(CXPS) == true)
//			{
//				FString CombinedMessageString = CXPS->PlayerNameString+ TEXT(" has won the game.");
//				ynPlayerController->NotificationText = FText::FromString(CombinedMessageString);
//
//				ResetGame();
//			}
//		}
//	}
//	else
//	{
//		bool bIsDraw = true;
//		for (const auto& ynPlayerController : AllPlayerControllers)
//		{
//			AynPlayerState* CXPS = ynPlayerController->GetPlayerState<AynPlayerState>();
//			if (IsValid(CXPS) == true)
//			{
//				if (CXPS->CurrentGuessCount < CXPS->MaxGuessCount)
//				{
//					bIsDraw = false;
//					break;
//				}
//			}
//		}
//		if (true == bIsDraw)
//		{
//			for (const auto& ynPlayerController : AllPlayerControllers)
//			{
//				ynPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));
//				ResetGame();
//			}
//		}
//	}
//}
	
void AynGameModeBase::JudgeGame(AynPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		// 3스트라이크 → 승리
		AynPlayerState* WinnerPS = InChattingPlayerController->GetPlayerState<AynPlayerState>();
		if (IsValid(WinnerPS) == true)
		{
			FString WinMessage = WinnerPS->PlayerNameString + TEXT(" has won the game!");

			for (const auto& ynPlayerController : AllPlayerControllers)
			{
				if (IsValid(ynPlayerController) == true)
				{
					// 서버에서 NotificationText 값을 바꾸면
					// ReplicatedUsing 덕분에 클라이언트에서 OnRep_NotificationText() 가 자동 호출돼요.
					// 단, 서버(ListenServer) 자신은 OnRep 이 안 불리므로 직접 호출해요.
					ynPlayerController->NotificationText = FText::FromString(WinMessage);
					if (ynPlayerController->HasAuthority())
					{
						ynPlayerController->OnRep_NotificationText();
					}
				}
			}
		}

		// 3초 뒤 리셋 (메시지를 볼 시간을 줘요)
		GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AynGameModeBase::ResetGame, 3.0f, false);
	}
	else
	{
		// 모두 기회를 다 썼는지 확인
		bool bIsDraw = true;
		for (const auto& ynPlayerController : AllPlayerControllers)
		{
			AynPlayerState* PS = ynPlayerController->GetPlayerState<AynPlayerState>();
			if (IsValid(PS) == true)
			{
				if (PS->CurrentGuessCount < PS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& ynPlayerController : AllPlayerControllers)
			{
				if (IsValid(ynPlayerController) == true)
				{
					ynPlayerController->NotificationText = FText::FromString(TEXT("Draw! Nobody got the answer."));
					if (ynPlayerController->HasAuthority())
					{
						ynPlayerController->OnRep_NotificationText();
					}
				}
			}

			// 3초 뒤 리셋
			GetWorldTimerManager().SetTimer(ResetTimerHandle, this, &AynGameModeBase::ResetGame, 3.0f, false);
		}
	}
}

	
