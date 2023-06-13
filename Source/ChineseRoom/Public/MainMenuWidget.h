// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */

 // Forward declaring classes
class UButton;


UCLASS()
class CHINESEROOM_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// Adds the menu to the player's screen and changes the input mode to UI
	UFUNCTION(BlueprintCallable)
	void MenuSetup();

protected:
	// Used to bind the buttons to their functions
	virtual bool Initialize() override;

	// Used to destroy the menu when we are done with it
	virtual void NativeDestruct() override;

private:
	// Pointer to the ContinueButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	// Pointer to the LevelsButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* LevelsButton;

	// Pointer to the HowToButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* HowToButton;

	// Pointer to the LearnMoreButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* LearnMoreButton;

	// Pointer to the SettingsButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	// Pointer to the QuitButton, must have the same name
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UFUNCTION()
	void ContinueButtonClicked();

	UFUNCTION()
	void LevelsButtonClicked();

	UFUNCTION()
	void HowToButtonClicked();

	UFUNCTION()
	void LearnMoreButtonClicked();

	UFUNCTION()
	void SettingsButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();
};
