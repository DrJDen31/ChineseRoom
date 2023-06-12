// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TheRoomMenu.generated.h"

/**
 * 
 */

// Forward declaring classes
class UButton;
class UTextBlock;

UCLASS()
class CHINESEROOM_API UTheRoomMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	// Adds the menu to the player's screen and changes the input mode to UI
	UFUNCTION(BlueprintCallable) // Allows the function to be called by Blueprints
	void MenuSetup(int NumberOfPages);

	// Tracks what page we are on, used to get the correct function
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // Allows the variable to be seen and edited by Blueprints
	int CurrentPage = 0;

	// Tracks the last page so we don't attempt to go past it
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int LastPage = 1;

	// Array of the left pages
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> LeftPages{ TEXT("Left page test text") };

	// Array of the right pages
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> RightPages{ TEXT("Right page test text") };

protected:
	// Used to bind the buttons to their functions
	virtual bool Initialize() override;

	// Used to destroy the menu when we are done with it
	virtual void NativeDestruct() override;

private:

	// Helper function to easily set the text of a TextBlock
	void SetTextBlockText(UTextBlock* InTextBlock, int InInteger);
	void SetTextBlockText(UTextBlock* InTextBlock, FString InString);


#pragma region Buttons
	// Pointer to the PrevPageButton, must have the same name
	UPROPERTY(meta = (BindWidget)) // Allows this variable to link to the one in blueprints
	UButton* PrevPageButton;
	
	// Pointer to the NextPageButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* NextPageButton;

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

	// Pointer to the WorkspaceText, must have the same name
	UPROPERTY(meta = (BindWidget))
	UTextBlock* WorkspaceText;
#pragma endregion
};
