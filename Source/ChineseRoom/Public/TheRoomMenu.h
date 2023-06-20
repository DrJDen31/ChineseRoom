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

UCLASS()
class CHINESEROOM_API UTheRoomMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	// Adds the menu to the player's screen and changes the input mode to UI
	UFUNCTION(BlueprintCallable) // Allows the function to be called by Blueprints
	void MenuSetup(TSubclassOf<UChineseRoomLevel> InLevel);

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

	// Int tracking the x position of the focus window
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FocusWindowXPosition = 0;

	// Int tracking the y position of the focus window
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FocusWindowYPosition = 0;

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

	UFUNCTION(BlueprintImplementableEvent)
	void SetWindowImage(UImage* InImage, EShapeSpecialCharacter Type);

private:
#pragma region Helpers
	// Helper function to easily set the text of a TextBlock
	void SetTextBlockText(UTextBlock* InTextBlock, int InInteger);
	void SetTextBlockText(UTextBlock* InTextBlock, FString InString);

	// Temporary until text blocks are replaced
	// Helper function to set the text using a window
	void SetTextBlockEnum(UTextBlock* InTextBlock, FWindow InWindow);

	// Helper to get string from enum, can be overloaded
	FString GetStringFromEnum(EShapeSpecialCharacter InEnum);

	// Helper function to manage switching books
	void HandleBookSwitched();
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

	// Pointer to the QuitButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
#pragma endregion

#pragma region Button OnClicked Functions
	//Function that will be called once the PrevPageButton is clicked
	UFUNCTION()
	void PrevPageButtonClicked();

	//Function that will be called once the NextPageButton is clicked
	UFUNCTION()
	void NextPageButtonClicked();

	//Function that will be called once the PrevBookButton is clicked
	UFUNCTION()
	void PrevBookButtonClicked();

	//Function that will be called once the NextBookButton is clicked
	UFUNCTION()
	void NextBookButtonClicked();

	//Function that will be called once the AutoSolveButton is clicked
	UFUNCTION()
	void AutoSolveButtonClicked();

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

	// Pointer to the WorkspaceText, must have the same name
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WorkspaceText;
#pragma endregion

#pragma region Images
	// Pointers to all of the images needed
	// Starts with Left Page, then Right Page, then Workspace

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

#pragma endregion

#pragma region Image Arrays
	// 2D Arrays holding all of the images needed to access by index
	TArray<FSpecialCharacterImageRow> LeftPageImages;
	TArray<FSpecialCharacterImageRow> RightPageImages;
	TArray<FSpecialCharacterImageRow> WorkspaceImages;

	void SetUpImageArrays();
	void SetImageArray(TArray<FSpecialCharacterImageRow> InImageArray, FWindow InWindow);

#pragma endregion

#pragma region SpecialCharacters
	// Variables pointing to the character textures
	UTexture2D* Circle;
	UTexture2D* Square;
	UTexture2D* Triangle;
#pragma endregion
};
