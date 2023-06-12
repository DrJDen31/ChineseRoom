// Fill out your copyright notice in the Description page of Project Settings.


#include "TheRoomMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"


void UTheRoomMenu::MenuSetup(int NumberOfPages)
{
	// Get the world
	UWorld* World = GetWorld();	
	// Verify it exists
	if (World)
	{
		// Get the player controller
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			// Set the player to interact with the UI rather than the game
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget()); 
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->SetShowMouseCursor(true);
		}
	}

	LastPage = NumberOfPages - 1; // Sets the number of pages

	if (PrevPageButton)
	{
		// Disable PrevPageButton since we default to the first page
		PrevPageButton->SetIsEnabled(false);
	}

	// Set the text for the first pages
	SetTextBlockText(PageNumberText, 0);
	SetTextBlockText(LeftPageText, LeftPages[0]);
	SetTextBlockText(RightPageText, FString::Printf(TEXT("Right page test text")));
	SetTextBlockText(WorkspaceText, FString::Printf(TEXT("Workspace test text")));
}

bool UTheRoomMenu::Initialize() 
{
	// Calls the super
	if (!Super::Initialize())
	{
		return false;
	}

	// Links the button to the functions
	if (PrevPageButton)
	{
		PrevPageButton->OnClicked.AddDynamic(this, &UTheRoomMenu::PrevPageButtonClicked);
	}

	if (NextPageButton)
	{
		NextPageButton->OnClicked.AddDynamic(this, &UTheRoomMenu::NextPageButtonClicked);
	}

	if (AutoSolveButton)
	{
		AutoSolveButton->OnClicked.AddDynamic(this, &UTheRoomMenu::AutoSolveButtonClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UTheRoomMenu::QuitButtonClicked);
	}

	return true;
}

void UTheRoomMenu::NativeDestruct()
{
	// Removes it from the player's screen
	RemoveFromParent();
}

void UTheRoomMenu::SetTextBlockText(UTextBlock* InTextBlock, int InInteger)
{
	if (InTextBlock)
	{
		InTextBlock->SetText(UKismetTextLibrary::Conv_IntToText(InInteger));
	}
}

void UTheRoomMenu::SetTextBlockText(UTextBlock* InTextBlock, FString InString)
{
	if (InTextBlock)
	{
		InTextBlock->SetText(FText::FromString(InString));
	}
}

void UTheRoomMenu::PrevPageButtonClicked()
{
	// Check to make sure there are previous pages to go to
	if (CurrentPage <= 0)
	{
		return;
	}

	// Decrement our page counter
	CurrentPage -= 1;

	// Make sure we have a page to reference
	if (LeftPages.Num() > CurrentPage)
	{
		// Update the text on the pages
		SetTextBlockText(LeftPageText, LeftPages[CurrentPage]);
	}
	else
	{
		// Otherwise just put blank page
		SetTextBlockText(LeftPageText, FString::Printf(TEXT("")));
	}

	// Make sure we have a page to reference
	if (RightPages.Num() > CurrentPage)
	{
		// Update the text on the pages
		SetTextBlockText(RightPageText, RightPages[CurrentPage]);
	}
	else
	{
		// Otherwise just put blank page
		SetTextBlockText(RightPageText, FString::Printf(TEXT("")));
	}
	
	// Update the page number text
	SetTextBlockText(PageNumberText, CurrentPage + 1);

	if (CurrentPage == 0 && PrevPageButton)
	{
		// Disable the PreviousPageButton if we are on the first page
		PrevPageButton->SetIsEnabled(false);
	}

	if ((!NextPageButton->GetIsEnabled()) && (CurrentPage < LastPage) && NextPageButton)
	{
		// Enable the NextPageButton if we aren't on the last page
		NextPageButton->SetIsEnabled(true);
	}

	// Easy way to print to the player's screen for debugging
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1, // Key: will replace any print with the same key
			5.f, // Duration to keep on screen
			FColor::Green, // Color
			FString::Printf(TEXT("PrevPageClicked")) // Message, works with same syntax as Printf in C
		);
	}
}

void UTheRoomMenu::NextPageButtonClicked()
{
	// Check to make sure there are more pages to go to
	if (CurrentPage >= LastPage)
	{
		return;
	}

	// Increment our page counter
	CurrentPage += 1;

	// Make sure we have a page to reference
	if (LeftPages.Num() > CurrentPage)
	{
		// Update the text on the pages
		SetTextBlockText(LeftPageText, LeftPages[CurrentPage]); 
	}
	else
	{
		// Otherwise just put blank page
		SetTextBlockText(LeftPageText, FString::Printf(TEXT("")));
	}

	// Make sure we have a page to reference
	if (RightPages.Num() > CurrentPage)
	{
		// Update the text on the pages
		SetTextBlockText(RightPageText, RightPages[CurrentPage]);
	}
	else
	{
		// Otherwise just put blank page
		SetTextBlockText(RightPageText, FString::Printf(TEXT("")));
	}

	// Update the page number text
	SetTextBlockText(PageNumberText, CurrentPage + 1);

	if (CurrentPage == LastPage && NextPageButton)
	{
		// Disable the NextPageButton if we are on the last page
		NextPageButton->SetIsEnabled(false);
	}

	if ((!PrevPageButton->GetIsEnabled()) && (CurrentPage > 0) && PrevPageButton)
	{
		// Enable the NextPageButton if we aren't on the first page
		PrevPageButton->SetIsEnabled(true);
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("NextPageClicked"))
		);
	}
}

void UTheRoomMenu::AutoSolveButtonClicked()
{
	// Where most of the implementation will be
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("AutoSolveClicked"))
		);
	}
}

void UTheRoomMenu::QuitButtonClicked()
{
	// Quits the game
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("QuitClicked"))
		);
	}
}