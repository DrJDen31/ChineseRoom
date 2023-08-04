// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChineseRoomTypes.h"
#include "TheRoomMenu.generated.h"

/**
 * 
 */

// Forward declaring classes
class UButton;
class UTextBlock;
class UImage;
class UBorder;

UCLASS()
class CHINESEROOM_API UTheRoomMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	// Adds the menu to the player's screen and changes the input mode to UI
	UFUNCTION(BlueprintCallable) // Allows the function to be called by Blueprints
	virtual void MenuSetup(TSubclassOf<UChineseRoomLevel> InLevel);

	// Array of the left pages
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> LeftPages{ TEXT("Left page test text") };

	// Array of the right pages
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> RightPages{ TEXT("Right page test text") };

#pragma region Contents
	// Main Bookshelf for the room
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FShelf Shelf;

	// Window corresponding to the current workspace
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWindow Workspace;

	// Window corresponding to the current focus window
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWindow FocusWindow;

	// Window corresponding to the desired output
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWindow DesiredOutput;

	// Text to display in the Help Menu
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HelpMenuText;
#pragma endregion

#pragma region Tracking variables
	// Tracks what page we are on, used to get the correct function
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // Allows the variable to be seen and edited by Blueprints
	int CurrentPage = 0;

	// Tracks the last page so we don't attempt to go past it
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int LastPage = 1;

	// Tracks what book we are on, used to get the correct function
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // Allows the variable to be seen and edited by Blueprints
	int CurrentBook = 0;

	// Tracks the last book so we don't attempt to go past it
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int LastBook = 1;

	// Int tracking the comlumn position of the focus window
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FocusWindowColumn = 0;

	// Int tracking the row position of the focus window
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FocusWindowRow = 0;

	// Bool tracking when we've solved the message
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsSolved = false;
#pragma endregion

#pragma region Sizes
	// Int describing the width of the rules on the pages
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int RuleWidth;

	// Int describing the height of the rules on the pages
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int RuleHeight;

	// Int describing the width of the workspace and desired output
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int WorkspaceWidth;

	// Int describing the height of the workspace and desired output
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int WorkspaceHeight;
#pragma endregion

protected:
	// Used to bind the buttons to their functions
	virtual bool Initialize() override;

	// Used to destroy the menu when we are done with it
	virtual void NativeDestruct() override;

	// Sets the image passed in to the shape passed in
	UFUNCTION(BlueprintImplementableEvent)
	void SetWindowImage(UImage* InImage, EShapeSpecialCharacter Type);

	// Updates the game state when the level has been completed
	UFUNCTION(BlueprintImplementableEvent)
	void OnLevelCompleted();

	// Moves the focus window by the desired offset
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void MoveFocusWindowBy(float X_Offset, float Y_Offset);

protected:
#pragma region Helpers
	// Helper function to easily set the text of a TextBlock
	void SetTextBlockText(UTextBlock* InTextBlock, int InInteger);
	void SetTextBlockText(UTextBlock* InTextBlock, FString InString);

	// Temporary until text blocks are replaced
	// Helper function to set the text using a window
	void SetTextBlockEnum(UTextBlock* InTextBlock, FWindow InWindow);

	// Helper to get string from enum, can be overloaded
	FString GetStringFromEnum(EShapeSpecialCharacter InEnum);
#pragma endregion

#pragma region Functionality
	// Function to manage switching books
	void HandleBookSwitched();

	// Moves the Focus Window
	void MoveFocusWindowTo(int Row, int Column);

	void MoveFocusWindowUI(int RowChange, int ColumnChange);

	// Checks if two windows are the same
	virtual bool AreWindowsIdentical(FWindow* Window1, FWindow* Window2);

	// Sets one window to be identical to the other
	void SetWindow(FWindow* WindowToChange, FWindow* TemplateWindow);

	// Sets one portion of a bigger window to a smaller one, starting at XOffset, YOffset
	void SetWindowPortion(FWindow* WindowToChange, FWindow* TemplateWindow, int RowOffset, int ColumnOffset);

	// Checks if the level has been beaten
	UFUNCTION(BlueprintCallable)
	bool CheckLevelCompleted();
#pragma endregion

#pragma region Buttons
	// Pointer to the PrevPageButton, must have the same name
	UPROPERTY(meta = (BindWidget)) // Allows this variable to link to the one in blueprints
	UButton* PrevPageButton;
	
	// Pointer to the NextPageButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* NextPageButton;

	// Pointer to the PrevBookButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* PrevBookButton;

	// Pointer to the NextBookButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* NextBookButton;

	// Pointer to the AutoSolveButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* AutoSolveButton;

	// Pointer to the FocusWindowLeftButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* FocusWindowLeftButton;

	// Pointer to the FocusWindowRightButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* FocusWindowRightButton;

	// Pointer to the FocusWindowUpButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* FocusWindowUpButton;

	// Pointer to the FocusWindowDownButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* FocusWindowDownButton;

	// Pointer to the QuitButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
#pragma endregion

#pragma region Button OnClicked Functions
	//Function that will be called once the PrevPageButton is clicked
	UFUNCTION()
	virtual void PrevPageButtonClicked();

	//Function that will be called once the NextPageButton is clicked
	UFUNCTION()
	virtual void NextPageButtonClicked();

	//Function that will be called once the PrevBookButton is clicked
	UFUNCTION()
	virtual void PrevBookButtonClicked();

	//Function that will be called once the NextBookButton is clicked
	UFUNCTION()
	virtual void NextBookButtonClicked();

	//Function that will be called once the AutoSolveButton is clicked
	UFUNCTION()
	virtual void AutoSolveButtonClicked();

	//Function that will be called once the FocusWindowLeftButton is clicked
	UFUNCTION()
	void FocusWindowLeftButtonClicked();

	//Function that will be called once the FocusWindowRightButton is clicked
	UFUNCTION()
	void FocusWindowRightButtonClicked();

	//Function that will be called once the FocusWindowUpButton is clicked
	UFUNCTION()
	void FocusWindowUpButtonClicked();

	//Function that will be called once the FocusWindowDownButton is clicked
	UFUNCTION()
	void FocusWindowDownButtonClicked();

	//Function that will be called once the QuitButton is clicked
	UFUNCTION()
	void QuitButtonClicked();
#pragma endregion

#pragma region Text Blocks
	// Pointer to the LeftPageText, must have the same name
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftPageText;

	// Pointer to the RightPageText, must have the same name
	UPROPERTY(meta = (BindWidget))
	UTextBlock* RightPageText;

	// Pointer to the PageNumberText, must have the same name
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PageNumberText;

	// Pointer to the BookNumberText, must have the same name
	UPROPERTY(meta = (BindWidget))
	UTextBlock* BookNumberText;
#pragma endregion

	/*
#pragma region Images
	// Pointers to all of the images needed
	// Then starts with Left Page, then Right Page, then Workspace

	//------- Left Page --------//

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left00;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left01;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left02;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left03;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left04;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left10;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left11;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left12;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left13;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left14;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left20;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left21;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left22;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left23;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left24;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left30;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left31;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left32;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left33;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left34;

	//------- Right Page --------//

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right00;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right01;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right02;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right03;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right04;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right10;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right11;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right12;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right13;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right14;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right20;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right21;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right22;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right23;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right24;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right30;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right31;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right32;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right33;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right34;

	//------- Workspace ---------//
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace00;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace01;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace02;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace03;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace04;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace05;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace06;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace07;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace08;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace09;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace10;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace11;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace12;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace13;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace14;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace15;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace16;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace17;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace18;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace19;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace20;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace21;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace22;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace23;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace24;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace25;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace26;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace27;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace28;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace29;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace30;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace31;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace32;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace33;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace34;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace35;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace36;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace37;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace38;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Workspace39;

#pragma endregion
*/

protected:

#pragma region Image Arrays
	// 2D Arrays holding all of the images needed to access by index
	TArray<FSpecialCharacterImageRow> LeftPageImages;
	TArray<FSpecialCharacterImageRow> RightPageImages;
	TArray<FSpecialCharacterImageRow> WorkspaceImages;

	float TileWidth = 80.0;
	float TileHeight = 75.0;

	virtual void SetUpImageArrays();
	virtual void SetImageArray(TArray<FSpecialCharacterImageRow> InImageArray, FWindow InWindow);

#pragma endregion

#pragma region SpecialCharacters
	// Variables pointing to the character textures
	UTexture2D* Circle;
	UTexture2D* Square;
	UTexture2D* Triangle;
	UTexture2D* Star;
#pragma endregion
};


