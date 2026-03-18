// CXChatInput.cpph


#include "UI/ynChatInput.h"
#include "Components/EditableTextBox.h"
#include "Player/ynPlayerController.h"

void UynChatInput::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == false)
	{
		EditableTextBox_ChatInput->OnTextCommitted.AddDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}

}

void UynChatInput::NativeDestruct()
{
	Super::NativeDestruct();

	if (EditableTextBox_ChatInput->OnTextCommitted.IsAlreadyBound(this, &ThisClass::OnChatInputTextCommitted) == true)
	{
		EditableTextBox_ChatInput->OnTextCommitted.RemoveDynamic(this, &ThisClass::OnChatInputTextCommitted);
	}
}

void UynChatInput::OnChatInputTextCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter)
	{
		APlayerController* OwningPlayerController = GetOwningPlayer();
		if (IsValid(OwningPlayerController) == true)
		{
			AynPlayerController* OwningynPlayerController = Cast<AynPlayerController>(OwningPlayerController);
			if (IsValid(OwningynPlayerController) == true)
			{
				OwningynPlayerController->SetChatMessageString(Text.ToString());
				EditableTextBox_ChatInput->SetText(FText());
			}
		}
	}
}
