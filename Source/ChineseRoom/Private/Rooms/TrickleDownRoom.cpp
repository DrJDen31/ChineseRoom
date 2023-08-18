// Fill out your copyright notice in the Description page of Project Settings.


#include "Rooms/TrickleDownRoom.h"
#include "Components/Button.h"


void UTrickleDownRoom::MenuSetup(TSubclassOf<UChineseRoomLevel> InLevel)
{
	Super::MenuSetup(InLevel);
	TileWidth = 170.0;
	TileHeight = 165.0;

	// Visually move the focus window to the starting position
	MoveFocusWindowBy(FMath::Clamp((WorkspaceImages[0].Row.Num() - Workspace.Contents[0].Row.Num()) / 2, 0, 6) * TileWidth,
					  FMath::Clamp((WorkspaceImages.Num() - Workspace.Contents.Num()) / 2, 0, 6) * TileHeight);
}

bool UTrickleDownRoom::Initialize()
{
	// Calls the super
	if (!Super::Initialize())
	{
		return false;
	}

	// Links the button to the functions
	if (RotateFocusWindowButton)
	{
		RotateFocusWindowButton->OnClicked.AddDynamic(this, &UTrickleDownRoom::RotateFocusWindowButtonClicked);
	}

	return true;
}

void UTrickleDownRoom::SetUpImageArrays()
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


	// Add the workspace images to the array
	FSpecialCharacterImageRow WorkspaceImageRow0;
	WorkspaceImageRow0.Row.Add(Image_Workspace00);
	WorkspaceImageRow0.Row.Add(Image_Workspace01);
	WorkspaceImageRow0.Row.Add(Image_Workspace02);
	WorkspaceImageRow0.Row.Add(Image_Workspace03);
	WorkspaceImageRow0.Row.Add(Image_Workspace04);
	WorkspaceImageRow0.Row.Add(Image_Workspace05);
	WorkspaceImages.Add(WorkspaceImageRow0);

	FSpecialCharacterImageRow WorkspaceImageRow1;
	WorkspaceImageRow1.Row.Add(Image_Workspace10);
	WorkspaceImageRow1.Row.Add(Image_Workspace11);
	WorkspaceImageRow1.Row.Add(Image_Workspace12);
	WorkspaceImageRow1.Row.Add(Image_Workspace13);
	WorkspaceImageRow1.Row.Add(Image_Workspace14);
	WorkspaceImageRow1.Row.Add(Image_Workspace15);
	WorkspaceImages.Add(WorkspaceImageRow1);

	FSpecialCharacterImageRow WorkspaceImageRow2;
	WorkspaceImageRow2.Row.Add(Image_Workspace20);
	WorkspaceImageRow2.Row.Add(Image_Workspace21);
	WorkspaceImageRow2.Row.Add(Image_Workspace22);
	WorkspaceImageRow2.Row.Add(Image_Workspace23);
	WorkspaceImageRow2.Row.Add(Image_Workspace24);
	WorkspaceImageRow2.Row.Add(Image_Workspace25);
	WorkspaceImages.Add(WorkspaceImageRow2);
}

void UTrickleDownRoom::SetImageArray(TArray<FSpecialCharacterImageRow> InImageArray, FWindow InWindow)
{
	if (InWindow.Contents[0].Row.Num() > InImageArray[0].Row.Num())
	{
		FWindow CroppedWindow;
		for (int j = 0; j < InImageArray.Num(); j++) 
		{
			FSpecialCharacterRow CroppedRow;
			for (int i = 0; i < InImageArray[j].Row.Num(); i++)
			{
				CroppedRow.Row.Add(InWindow.Contents[j].Row[i]);
			}
			CroppedWindow.Contents.Add(CroppedRow);
		}

		Super::SetImageArray(InImageArray, CroppedWindow);
		return;
	}
	Super::SetImageArray(InImageArray, InWindow);
}

void UTrickleDownRoom::ShiftWorkspace(bool ToTheLeft)
{
	//ToTheLeft describes if the workspace seems to physically move to the left when we shift it
	//This would occur when trying to move the focus window further to the right

	int offset = 1;
	if (!ToTheLeft)
	{
		offset = -1;
	}

	WorkspaceOffset += offset;

	for (int i = 0; i < WorkspaceImages.Num(); i++) {
		for (int j = 0; j < WorkspaceImages[i].Row.Num(); j++)
		{
			EShapeSpecialCharacter CurrentShape = Workspace.Contents[i].Row[j + WorkspaceOffset];
			UImage* CurrentImage = WorkspaceImages[i].Row[j];
			SetWindowImage(CurrentImage, CurrentShape);
		}
	}

	//SetImageArray(WorkspaceImages, Workspace);
}

void UTrickleDownRoom::RotateFocusWindowButtonClicked()
{
	if(GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5.f,
			FColor::Green,
			FString::Printf(TEXT("Rotate Focus Window Clicked"))
		);
	}
}

void UTrickleDownRoom::AutoSolveButtonClicked()
{

}

void UTrickleDownRoom::FocusWindowLeftButtonClicked()
{
	if (FocusWindowColumn > 0)
	{
		MoveFocusWindowTo(FocusWindowRow, FocusWindowColumn - 1);
	}
	else if (WorkspaceOffset > 0)
	{
		ShiftWorkspace(false);
	}
}

void UTrickleDownRoom::FocusWindowRightButtonClicked()
{
	if (FocusWindowColumn + FocusWindow.Width < WorkspaceImages[0].Row.Num())
	{
		MoveFocusWindowTo(FocusWindowRow, FocusWindowColumn + 1);
	}
	else if (WorkspaceOffset < Workspace.Contents[0].Row.Num() - 6)
	{
		ShiftWorkspace(true);
	}
}