// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/LinearAdditionRoom.h"

void ULinearAdditionRoom::MenuSetup(TSubclassOf<UChineseRoomLevel> InLevel)
{
	Super::MenuSetup(InLevel);
	TileWidth = 160.0;
	TileHeight = 150.0;
	RuleHasOptions(DoesRuleHaveOptions());
}

void ULinearAdditionRoom::SetUpImageArrays()
{
	// Add the left page images to the array
	FSpecialCharacterImageRow LeftPageImageRow0;
	LeftPageImageRow0.Row.Add(Image_Left00);
	LeftPageImageRow0.Row.Add(Image_Left01);
	LeftPageImages.Add(LeftPageImageRow0);

	FSpecialCharacterImageRow LeftPageImageRow1;
	LeftPageImageRow1.Row.Add(Image_Left10);
	LeftPageImageRow1.Row.Add(Image_Left11);
	LeftPageImages.Add(LeftPageImageRow1);

	// Add the right page images to the array
	FSpecialCharacterImageRow RightPageImageRow0;
	RightPageImageRow0.Row.Add(Image_Right00);
	RightPageImageRow0.Row.Add(Image_Right01);
	RightPageImages.Add(RightPageImageRow0);

	FSpecialCharacterImageRow RightPageImageRow1;
	RightPageImageRow1.Row.Add(Image_Right10);
	RightPageImageRow1.Row.Add(Image_Right11);
	RightPageImages.Add(RightPageImageRow1);

	FSpecialCharacterImageRow WorkspaceImageRow;
	WorkspaceImageRow.Row.Add(Image_Workspace00);
	WorkspaceImageRow.Row.Add(Image_Workspace01);
	WorkspaceImageRow.Row.Add(Image_Workspace02);
	WorkspaceImageRow.Row.Add(Image_Workspace03);
	WorkspaceImageRow.Row.Add(Image_Workspace04);
	WorkspaceImageRow.Row.Add(Image_Workspace05);
	WorkspaceImages.Add(WorkspaceImageRow);
}

void ULinearAdditionRoom::SetImageArray(TArray<FSpecialCharacterImageRow> InImageArray, FWindow InWindow)
{
	if (InWindow.Contents[0].Row.Num() > InImageArray[0].Row.Num())
	{
		FWindow CroppedWindow; 
		FSpecialCharacterRow CroppedRow;
		for (int i = 0; i < InImageArray[0].Row.Num(); i++)
		{
			CroppedRow.Row.Add(InWindow.Contents[0].Row[i]);
		}
		CroppedWindow.Contents.Add(CroppedRow);
		Super::SetImageArray(InImageArray, CroppedWindow);
		return;
	}
	Super::SetImageArray(InImageArray, InWindow);
}

int ULinearAdditionRoom::DoesRuleApply()
{
	bool Applies = true;
	for (int i = 0; i < 2; i++)
	{
		if (FocusWindow.Contents[0].Row[i] != Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage.Contents[0].Row[i])
		{
			Applies = false;
		}
	}

	if (Applies)
	{
		return 1;
	}

	if (Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage.Contents.Num() == 1)
	{
		return 0;
	}
	Applies = true;
	for (int j = 0; j < 2; j++)
	{
		if (FocusWindow.Contents[0].Row[j] != Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage.Contents[1].Row[j])
		{
			Applies = false;
		}
	}

	if (Applies)
	{
		return 2;
	}

	return 0;
}

void ULinearAdditionRoom::ApplyRule(int Rule)
{
	Workspace.Contents[0].Row[FocusWindowColumn] = Shelf.Books[CurrentBook].Pages[CurrentPage].RightPage.Contents[0].Row[0];
}

void ULinearAdditionRoom::ShiftWorkspace()
{
	for (int i = FocusWindowColumn + 1; i < Workspace.Contents[0].Row.Num() - 1; i++) 
	{
		Workspace.Contents[0].Row[i] = Workspace.Contents[0].Row[i + 1];
	}

	Workspace.Contents[0].Row[Workspace.Contents[0].Row.Num() - 1] = EShapeSpecialCharacter::Blank;
	SetImageArray(WorkspaceImages, Workspace);
}

void ULinearAdditionRoom::AutoSolveButtonClicked()
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

	if (Shelf.Books[CurrentBook].Pages[CurrentPage].SpecialRule == ESpecialRule::FocusWindowReachedEnd) 
	{
		if (FocusWindow.Contents[0].Row[1] == EShapeSpecialCharacter::Blank)
		{
			OnLevelCompleted();
		}
		return;
	}

	int Check = DoesRuleApply();
	if (Check > 0)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Green,
				FString::Printf(TEXT("Identical with rule %d"), Check)
			);
		}
		ApplyRule(Check);
		ShiftWorkspace();
		MoveFocusWindowTo(FocusWindowRow, FocusWindowColumn + 1);

		if (Shelf.Books[CurrentBook].Pages[CurrentPage].SpecialRule == ESpecialRule::NextBook) 
		{
			NextBookButtonClicked();
		}
		else if (Shelf.Books[CurrentBook].Pages[CurrentPage].SpecialRule == ESpecialRule::PrevBook)
		{
			PrevBookButtonClicked();
		}

		// AutoSolve
		/*
		if (FocusWindow.Contents[0].Row[0] == EShapeSpecialCharacter::Blank && FocusWindow.Contents[0].Row[1] == EShapeSpecialCharacter::Blank)
		{
			OnLevelCompleted();
		}
		*/
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				5.f,
				FColor::Green,
				FString::Printf(TEXT("Nope, try again"))
			);
		}
	}
}


bool ULinearAdditionRoom::DoesRuleHaveOptions()
{
	return (Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage.Contents.Num() > 1)
		&& (Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage.Contents[1].Row[0] != EShapeSpecialCharacter::None)
		&& (Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage.Contents[1].Row[1] != EShapeSpecialCharacter::None);
}

void ULinearAdditionRoom::PrevPageButtonClicked()
{
	Super::PrevPageButtonClicked();
	RuleHasOptions(DoesRuleHaveOptions());
}

void ULinearAdditionRoom::NextPageButtonClicked()
{
	Super::NextPageButtonClicked();
	RuleHasOptions(DoesRuleHaveOptions());
}

void ULinearAdditionRoom::PrevBookButtonClicked()
{
	Super::PrevBookButtonClicked();
	RuleHasOptions(DoesRuleHaveOptions());
}

void ULinearAdditionRoom::NextBookButtonClicked()
{
	Super::NextBookButtonClicked();
	RuleHasOptions(DoesRuleHaveOptions());
}
