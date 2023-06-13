// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSelectWidget.h"

void ULevelSelectWidget::MenuSetup()
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
}

bool ULevelSelectWidget::Initialize()
{
	// Calls the super
	if (!Super::Initialize())
	{
		return false;
	}

	return true;
}

void ULevelSelectWidget::NativeDestruct()
{
	RemoveFromParent();
}