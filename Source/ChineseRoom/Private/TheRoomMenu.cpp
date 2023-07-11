// Fill out your copyright notice in the Description page of Project Settings.


#include "TheRoomMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
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
	SetImageArray(WorkspaceImages, Workspace);

	// Moves the focus window to the centered position
	MoveFocusWindowTo((WorkspaceImages.Num() - Workspace.Contents.Num()) / 2, (WorkspaceImages[0].Row.Num() - Workspace.Contents[0].Row.Num()) / 2);
	
	// Reset these to 0 as we updated the default position
	FocusWindowRow = 0;
	FocusWindowColumn = 0;
	
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

#pragma region boilerplate
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

	if (FocusWindowLeftButton)
	{
		FocusWindowLeftButton->OnClicked.AddDynamic(this, &UTheRoomMenu::FocusWindowLeftButtonClicked);
	}

	if (FocusWindowRightButton)
	{
		FocusWindowRightButton->OnClicked.AddDynamic(this, &UTheRoomMenu::FocusWindowRightButtonClicked);
	}

	if (FocusWindowUpButton)
	{
		FocusWindowUpButton->OnClicked.AddDynamic(this, &UTheRoomMenu::FocusWindowUpButtonClicked);
	}

	if (FocusWindowDownButton)
	{
		FocusWindowDownButton->OnClicked.AddDynamic(this, &UTheRoomMenu::FocusWindowDownButtonClicked);
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
#pragma endregion

#pragma region Helpers
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

	// Add the workspace images to the array
	FSpecialCharacterImageRow WorkspaceImageRow0;
	WorkspaceImageRow0.Row.Add(Image_Workspace00);
	WorkspaceImageRow0.Row.Add(Image_Workspace01);
	WorkspaceImageRow0.Row.Add(Image_Workspace02);
	WorkspaceImageRow0.Row.Add(Image_Workspace03);
	WorkspaceImageRow0.Row.Add(Image_Workspace04);
	WorkspaceImageRow0.Row.Add(Image_Workspace05);
	WorkspaceImageRow0.Row.Add(Image_Workspace06);
	WorkspaceImageRow0.Row.Add(Image_Workspace07);
	WorkspaceImageRow0.Row.Add(Image_Workspace08);
	WorkspaceImageRow0.Row.Add(Image_Workspace09);
	WorkspaceImages.Add(WorkspaceImageRow0);

	FSpecialCharacterImageRow WorkspaceImageRow1;
	WorkspaceImageRow1.Row.Add(Image_Workspace10);
	WorkspaceImageRow1.Row.Add(Image_Workspace11);
	WorkspaceImageRow1.Row.Add(Image_Workspace12);
	WorkspaceImageRow1.Row.Add(Image_Workspace13);
	WorkspaceImageRow1.Row.Add(Image_Workspace14);
	WorkspaceImageRow1.Row.Add(Image_Workspace15);
	WorkspaceImageRow1.Row.Add(Image_Workspace16);
	WorkspaceImageRow1.Row.Add(Image_Workspace17);
	WorkspaceImageRow1.Row.Add(Image_Workspace18);
	WorkspaceImageRow1.Row.Add(Image_Workspace19);
	WorkspaceImages.Add(WorkspaceImageRow1);

	FSpecialCharacterImageRow WorkspaceImageRow2;
	WorkspaceImageRow2.Row.Add(Image_Workspace20);
	WorkspaceImageRow2.Row.Add(Image_Workspace21);
	WorkspaceImageRow2.Row.Add(Image_Workspace22);
	WorkspaceImageRow2.Row.Add(Image_Workspace23);
	WorkspaceImageRow2.Row.Add(Image_Workspace24);
	WorkspaceImageRow2.Row.Add(Image_Workspace25);
	WorkspaceImageRow2.Row.Add(Image_Workspace26);
	WorkspaceImageRow2.Row.Add(Image_Workspace27);
	WorkspaceImageRow2.Row.Add(Image_Workspace28);
	WorkspaceImageRow2.Row.Add(Image_Workspace29);
	WorkspaceImages.Add(WorkspaceImageRow2);

	FSpecialCharacterImageRow WorkspaceImageRow3;
	WorkspaceImageRow3.Row.Add(Image_Workspace30);
	WorkspaceImageRow3.Row.Add(Image_Workspace31);
	WorkspaceImageRow3.Row.Add(Image_Workspace32);
	WorkspaceImageRow3.Row.Add(Image_Workspace33);
	WorkspaceImageRow3.Row.Add(Image_Workspace34);
	WorkspaceImageRow3.Row.Add(Image_Workspace35);
	WorkspaceImageRow3.Row.Add(Image_Workspace36);
	WorkspaceImageRow3.Row.Add(Image_Workspace37);
	WorkspaceImageRow3.Row.Add(Image_Workspace38);
	WorkspaceImageRow3.Row.Add(Image_Workspace39);
	WorkspaceImages.Add(WorkspaceImageRow3);
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

	int HeightOffset = (InImageArray.Num() - Height) / 2;
	int WidthOffset = (InImageArray[0].Row.Num() - Width) / 2;

	for (int i = 0; i < InWindow.Contents.Num(); i++)
	{
		for (int j = 0; j < InWindow.Contents[i].Row.Num(); j++)
		{
			EShapeSpecialCharacter CurrentShape = InWindow.Contents[i].Row[j];
			UImage* CurrentImage = InImageArray[i + HeightOffset].Row[j + WidthOffset]; // Out of bounds error on the width
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
  else if (InEnum == EShapeSpecialCharacter::Star)
	{
		StringToReturn = TEXT("R");
	}
	return StringToReturn;
}
#pragma endregion

#pragma region Functionality
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

void UTheRoomMenu::MoveFocusWindowTo(int Row, int Column)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString::Printf(TEXT("Moving to %d, %d"), Column, Row)
		);
	}
	// Check if the position is in range -- check to make indices are correct
	if (Row < 0 || Row + FocusWindowRow > Workspace.Height || Column < 0 || Column + FocusWindowColumn > Workspace.Width)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Yellow,
				FString::Printf(TEXT("Out of bounds, Row: %d, Column: %d, FWR: %d, FWC: %d, Height: %d, Width: %d"), Row, Column, FocusWindowRow, FocusWindowColumn, Workspace.Height, Workspace.Width)
			);
		}
		return;
	}

	// Set RowOffset and ColumnOffset to new - old
	int RowOffset = Row - FocusWindowRow;
	int ColumnOffset = Column - FocusWindowColumn;

	// Update focus window position variables
	FocusWindowRow = Row;
	FocusWindowColumn = Column;

	// Go through each spot in the focus window and set it to the value
	// in the corresponding part of the workspace
	// ****** STILL TO BE IMPLEMENTED *********** //

	MoveFocusWindowUI(RowOffset, ColumnOffset);
}

void UTheRoomMenu::MoveFocusWindowUI(int RowChange, int ColumnChange)
{
	// Temporary fix to move it by the correct amount based on the approximate size of the pictures
	float TileWidth = 80.0;
	float TileHeight = 75.0;

	// Call set position of focus window image,
	// new position = old + (const * change)
	// where const = width of one grid block
	// and change is the row or column change passed in

	MoveFocusWindowBy(ColumnChange * TileWidth, RowChange * TileHeight);
}

bool UTheRoomMenu::AreWindowsIdentical(FWindow* Window1, FWindow* Window2)
{
	// Check to see if sizes match

	// Go through each spot in window1 and make sure its identical to the
	// corresponding spot in window2
	
	return true;
}

void UTheRoomMenu::SetWindow(FWindow* WindowToChange, FWindow* TemplateWindow)
{
	// Check to see if sizes match

	// Go through each spot in window to change and set it to the corresponding
	// spot in the template window
}

void UTheRoomMenu::SetWindowPortion(FWindow* WindowToChange, FWindow* TemplateWindow, int RowOffset, int ColumnOffset)
{
	// Make sure the template + the offset is still in range

	// Go through each spot in the template and set the corresponding part in window to change
	// + the offset to the new value
}

bool UTheRoomMenu::CheckLevelCompleted()
{
	// Check to see if the Workspace == Desired Output and any other necessary 
	// conditions to complete the level

	// If so, call OnLevelCompleted();
	// and return true;

	// If not,
	return false;
}
#pragma endregion

#pragma region Button Functionality
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

void UTheRoomMenu::FocusWindowLeftButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString::Printf(TEXT("Left"))
		);
	}
	if (FocusWindowColumn > 0)
	{
		MoveFocusWindowTo(FocusWindowRow, FocusWindowColumn - 1);
	}
}

void UTheRoomMenu::FocusWindowRightButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString::Printf(TEXT("Right"))
		);
	}
	if (FocusWindowColumn + FocusWindow.Width < Workspace.Width)
	{
		MoveFocusWindowTo(FocusWindowRow, FocusWindowColumn + 1);
	}
}

void UTheRoomMenu::FocusWindowUpButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString::Printf(TEXT("Up"))
		);
	}
	if (FocusWindowRow > 0)
	{
		MoveFocusWindowTo(FocusWindowRow - 1, FocusWindowColumn);
	}
}

void UTheRoomMenu::FocusWindowDownButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Yellow,
			FString::Printf(TEXT("Down"))
		);
	}

	if (FocusWindowRow + FocusWindow.Height < Workspace.Height)
	{
		MoveFocusWindowTo(FocusWindowRow + 1, FocusWindowColumn);
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
#pragma endregion