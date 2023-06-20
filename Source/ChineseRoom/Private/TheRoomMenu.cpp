// Fill out your copyright notice in the Description page of Project Settings.


#include "TheRoomMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"


void UTheRoomMenu::MenuSetup(TSubclassOf<UChineseRoomLevel> InLevel)
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

	// Set the shape textures - was used to set texture using c++, switched to Blueprint
	/*
	Circle = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString("/Game/SpecialCharacters/Shapes/Circle.uasset")));;
	Square = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString("/Game/SpecialCharacters/Shapes/Square.uasset")));;
	Triangle = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, *FString("/Game/SpecialCharacters/Shapes/Triangle.uasset")));;
	*/

	// Set the shelf and windows
	Shelf = InLevel.GetDefaultObject()->Room.GetDefaultObject()->Shelf;
	Workspace = InLevel.GetDefaultObject()->StartingWorkspace;
	FocusWindow = InLevel.GetDefaultObject()->FocusWindow;
	DesiredOutput = InLevel.GetDefaultObject()->DesiredOutput;
	HelpMenuText = InLevel.GetDefaultObject()->Room.GetDefaultObject()->HelpText;

	// Set the number of books
	LastBook = Shelf.Books.Num() - 1;

	// Initialize Arrays
	SetUpImageArrays();

	// Set the workspace - eventually switch to image array
	SetTextBlockEnum(WorkspaceText, Workspace);

	// Handle "switching" to the first book
	HandleBookSwitched();

	if (PrevBookButton)
	{
		// On the first book, disable prev book button
		PrevBookButton->SetIsEnabled(false);
	}

	if (NextBookButton && Shelf.Books.Num() == 1)
	{
		// Only 1 book, disable next book button
		NextBookButton->SetIsEnabled(false);
	}
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

	if (PrevBookButton)
	{
		PrevBookButton->OnClicked.AddDynamic(this, &UTheRoomMenu::PrevBookButtonClicked);
	}

	if (NextBookButton)
	{
		NextBookButton->OnClicked.AddDynamic(this, &UTheRoomMenu::NextBookButtonClicked);
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

void UTheRoomMenu::SetTextBlockEnum(UTextBlock* InTextBlock, FWindow InWindow)
{
	// Text that we will be setting
	FString TextToSet;

	// Go through each slot of the window
	for (int i = 0; i < InWindow.Contents.Num(); i++)
	{
		for (int j = 0; j < InWindow.Contents[i].Row.Num(); j++)
		{
			// Append a string depending on the value of the enum at this slot
			FString TextToAdd;
			EShapeSpecialCharacter CurrentShape = InWindow.Contents[i].Row[j];
			TextToAdd = GetStringFromEnum(CurrentShape);
			TextToSet += TextToAdd;
		}
		// Add a new line
		TextToSet += TEXT("\n");
	}

	// Set the end product to the textblock
	SetTextBlockText(InTextBlock, TextToSet);
}

void UTheRoomMenu::SetUpImageArrays()
{
	// Add the left page images to the array
	FSpecialCharacterImageRow LeftPageImageRow0;
	LeftPageImageRow0.Row.Add(Image_Left00);
	LeftPageImageRow0.Row.Add(Image_Left01);
	LeftPageImageRow0.Row.Add(Image_Left02);
	LeftPageImageRow0.Row.Add(Image_Left03);
	LeftPageImageRow0.Row.Add(Image_Left04);
	LeftPageImages.Add(LeftPageImageRow0);

	FSpecialCharacterImageRow LeftPageImageRow1;
	LeftPageImageRow1.Row.Add(Image_Left10);
	LeftPageImageRow1.Row.Add(Image_Left11);
	LeftPageImageRow1.Row.Add(Image_Left12);
	LeftPageImageRow1.Row.Add(Image_Left13);
	LeftPageImageRow1.Row.Add(Image_Left14);
	LeftPageImages.Add(LeftPageImageRow1);

	FSpecialCharacterImageRow LeftPageImageRow2;
	LeftPageImageRow2.Row.Add(Image_Left20);
	LeftPageImageRow2.Row.Add(Image_Left21);
	LeftPageImageRow2.Row.Add(Image_Left22);
	LeftPageImageRow2.Row.Add(Image_Left23);
	LeftPageImageRow2.Row.Add(Image_Left24);
	LeftPageImages.Add(LeftPageImageRow2);

	FSpecialCharacterImageRow LeftPageImageRow3;
	LeftPageImageRow3.Row.Add(Image_Left30);
	LeftPageImageRow3.Row.Add(Image_Left31);
	LeftPageImageRow3.Row.Add(Image_Left32);
	LeftPageImageRow3.Row.Add(Image_Left33);
	LeftPageImageRow3.Row.Add(Image_Left34);
	LeftPageImages.Add(LeftPageImageRow3);

	// Add the right page images to the array
	FSpecialCharacterImageRow RightPageImageRow0;
	RightPageImageRow0.Row.Add(Image_Right00);
	RightPageImageRow0.Row.Add(Image_Right01);
	RightPageImageRow0.Row.Add(Image_Right02);
	RightPageImageRow0.Row.Add(Image_Right03);
	RightPageImageRow0.Row.Add(Image_Right04);
	RightPageImages.Add(RightPageImageRow0);

	FSpecialCharacterImageRow RightPageImageRow1;
	RightPageImageRow1.Row.Add(Image_Right10);
	RightPageImageRow1.Row.Add(Image_Right11);
	RightPageImageRow1.Row.Add(Image_Right12);
	RightPageImageRow1.Row.Add(Image_Right13);
	RightPageImageRow1.Row.Add(Image_Right14);
	RightPageImages.Add(RightPageImageRow1);

	FSpecialCharacterImageRow RightPageImageRow2;
	RightPageImageRow2.Row.Add(Image_Right20);
	RightPageImageRow2.Row.Add(Image_Right21);
	RightPageImageRow2.Row.Add(Image_Right22);
	RightPageImageRow2.Row.Add(Image_Right23);
	RightPageImageRow2.Row.Add(Image_Right24);
	RightPageImages.Add(RightPageImageRow2);

	FSpecialCharacterImageRow RightPageImageRow3;
	RightPageImageRow3.Row.Add(Image_Right30);
	RightPageImageRow3.Row.Add(Image_Right31);
	RightPageImageRow3.Row.Add(Image_Right32);
	RightPageImageRow3.Row.Add(Image_Right33);
	RightPageImageRow3.Row.Add(Image_Right34);
	RightPageImages.Add(RightPageImageRow3);
}

void UTheRoomMenu::SetImageArray(TArray<FSpecialCharacterImageRow> InImageArray, FWindow InWindow)
{
	// Go through each slot of the window
	int Height = InWindow.Contents.Num();
	int Width = 0; 

	if (Height > 0)
	{
		Width = InWindow.Contents[0].Row.Num();
	}

	int HeightOffset = (4 - Height) / 2;
	int WidthOffset = (5 - Width) / 2;

	for (int i = 0; i < InWindow.Contents.Num(); i++)
	{
		for (int j = 0; j < InWindow.Contents[i].Row.Num(); j++)
		{
			EShapeSpecialCharacter CurrentShape = InWindow.Contents[i].Row[j];
			UImage* CurrentImage = InImageArray[i+WidthOffset].Row[j+HeightOffset];
			SetWindowImage(CurrentImage, CurrentShape);
		}
	}
}

FString UTheRoomMenu::GetStringFromEnum(EShapeSpecialCharacter InEnum)
{
	// Return a string depending on the value of the enum
	FString StringToReturn = TEXT("");
	if (InEnum == EShapeSpecialCharacter::Circle)
	{
		StringToReturn = TEXT("C");
	}
	else if (InEnum == EShapeSpecialCharacter::Triangle)
	{
		StringToReturn = TEXT("T");
	}
	else if (InEnum == EShapeSpecialCharacter::Square)
	{
		StringToReturn = TEXT("S");
	}
	return StringToReturn;
}

void UTheRoomMenu::HandleBookSwitched() 
{
	// If we have at least one book, set the number of pages
	if (LastBook > 0)
	{
		LastPage = Shelf.Books[CurrentBook].Pages.Num() - 1;
	}

	if (PrevPageButton)
	{
		// Disable PrevPageButton since we default to the first page
		PrevPageButton->SetIsEnabled(false);
	}

	if (NextPageButton && Shelf.Books.Num() > 0 && Shelf.Books[0].Pages.Num() == 1)
	{
		// Disable NextPageButton since there is only one page
		NextPageButton->SetIsEnabled(false);
	}
	else if (NextPageButton && Shelf.Books.Num() > 1 && !(NextPageButton->GetIsEnabled()))
	{
		NextPageButton->SetIsEnabled(true);
	}

	// Set the text for the first pages
	SetTextBlockText(PageNumberText, 1);
	SetTextBlockText(BookNumberText, CurrentBook + 1);

	// Set the image arrays
	SetImageArray(LeftPageImages, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage);
	SetImageArray(RightPageImages, Shelf.Books[CurrentBook].Pages[CurrentPage].RightPage);
	
	// Set the text part of the rule
	SetTextBlockText(LeftPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftRuleText);
	SetTextBlockText(RightPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].RightRuleText);
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
	if (Shelf.Books[CurrentBook].Pages.Num() > CurrentPage)
	{
		// Update the text on the pages -- old way
		//SetTextBlockEnum(LeftPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage);
		//SetTextBlockEnum(RightPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].RightPage);

		// Update the text on the pages -- new way
		SetTextBlockText(LeftPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftRuleText);
		SetTextBlockText(RightPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].RightRuleText);

		// Update the image array
		SetImageArray(LeftPageImages, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage);
		SetImageArray(RightPageImages, Shelf.Books[CurrentBook].Pages[CurrentPage].RightPage);
	}
	else
	{
		// Otherwise just put blank page
		SetTextBlockText(LeftPageText, FString::Printf(TEXT("")));
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
	if (Shelf.Books[CurrentBook].Pages.Num() > CurrentPage)
	{
		// Update the text on the pages -- old way
		//SetTextBlockEnum(LeftPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage);
		//SetTextBlockEnum(RightPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].RightPage);

		// Update the text on the pages -- new way
		SetTextBlockText(LeftPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftRuleText);
		SetTextBlockText(RightPageText, Shelf.Books[CurrentBook].Pages[CurrentPage].RightRuleText);

		// Update the image array
		SetImageArray(LeftPageImages, Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage);
		SetImageArray(RightPageImages, Shelf.Books[CurrentBook].Pages[CurrentPage].RightPage);
	}
	else
	{
		// Otherwise just put blank page
		SetTextBlockText(LeftPageText, FString::Printf(TEXT("")));
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

void UTheRoomMenu::PrevBookButtonClicked()
{
	// Check to make sure there are previous books to go to
	if (CurrentBook <= 0)
	{
		return;
	}

	// Decrement our book counter
	CurrentBook -= 1;

	// Set our current page back to the beginning: 0
	CurrentPage = 0;

	if (CurrentBook == 0 && PrevBookButton)
	{
		// Disable the PreviousBookButton if we are on the first book
		PrevBookButton->SetIsEnabled(false);
	}

	if ((!NextBookButton->GetIsEnabled()) && (CurrentBook < LastBook) && NextBookButton)
	{
		// Enable the NextBookButton if we aren't on the last book
		NextBookButton->SetIsEnabled(true);
	}

	HandleBookSwitched();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("PrevBookClicked"))
		);
	}
}

void UTheRoomMenu::NextBookButtonClicked()
{
	// Check to make sure there are more books to go to
	if (CurrentBook >= LastBook)
	{
		return;
	}

	// Increment our book counter
	CurrentBook += 1;

	// Set our current page back to the beginning: 0
	CurrentPage = 0;

	if (CurrentBook == LastBook && NextBookButton)
	{
		// Disable the NextPageButton if we are on the last book
		NextBookButton->SetIsEnabled(false);
	}

	if ((!PrevBookButton->GetIsEnabled()) && (CurrentBook > 0) && PrevBookButton)
	{
		// Enable the PrevPageButton if we aren't on the first book
		PrevBookButton->SetIsEnabled(true);
	}

	HandleBookSwitched();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("PrevBookClicked"))
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