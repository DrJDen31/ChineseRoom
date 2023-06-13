// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelectWidget.generated.h"

/**
 * 
 */
UCLASS()
class CHINESEROOM_API ULevelSelectWidget : public UUserWidget
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
};
