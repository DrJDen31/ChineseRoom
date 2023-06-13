// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetTextLibrary.h"

void UMainMenuWidget::MenuSetup()
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

bool UMainMenuWidget::Initialize()
{
	// Calls the super
	if (!Super::Initialize())
	{
		return false;
	}

	// Links the button to the functions
	if (ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this, &UMainMenuWidget::ContinueButtonClicked);
	}

	if (LevelsButton)
	{
		LevelsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LevelsButtonClicked);
	}

	if (HowToButton)
	{
		HowToButton->OnClicked.AddDynamic(this, &UMainMenuWidget::HowToButtonClicked);
	}

	if (LearnMoreButton)
	{
		LearnMoreButton->OnClicked.AddDynamic(this, &UMainMenuWidget::LearnMoreButtonClicked);
	}

	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::SettingsButtonClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::QuitButtonClicked);
	}

	return true;
}

void UMainMenuWidget::NativeDestruct()
{
	RemoveFromParent();
}


void UMainMenuWidget::ContinueButtonClicked()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("ContinueClicked"))
		);
	}
}

void UMainMenuWidget::LevelsButtonClicked()
{

}

void UMainMenuWidget::HowToButtonClicked()
{

}

void UMainMenuWidget::LearnMoreButtonClicked()
{

}

void UMainMenuWidget::SettingsButtonClicked()
{

}

void UMainMenuWidget::QuitButtonClicked()
{
	// Quits the game
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}