// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/LinearAdditionRoom.h"

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
	Super::SetImageArray(InImageArray, InWindow);
}

int ULinearAdditionRoom::DoesRuleApply()
{
	bool Applies = true;
	for (int i = 0; i < FocusWindow.Contents[0].Row.Num(); i++)
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

	for (int i = 0; i < FocusWindow.Contents[0].Row.Num(); i++)
	{
		if (FocusWindow.Contents[0].Row[i] != Shelf.Books[CurrentBook].Pages[CurrentPage].LeftPage.Contents[1].Row[i])
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

}

void ULinearAdditionRoom::ShiftWorkspace()
{

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
		MoveFocusWindowTo(FocusWindowRow, FocusWindowColumn);
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