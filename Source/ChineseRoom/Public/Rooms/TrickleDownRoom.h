// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheRoomMenu.h"
#include "TrickleDownRoom.generated.h"

/**
 * 
 */
UCLASS()
class CHINESEROOM_API UTrickleDownRoom : public UTheRoomMenu
{
	GENERATED_BODY()
	
public:
	virtual void MenuSetup(TSubclassOf<UChineseRoomLevel> InLevel) override;

protected:

	virtual void SetUpImageArrays() override;
	virtual void SetImageArray(TArray<FSpecialCharacterImageRow> InImageArray, FWindow InWindow) override;

	//Function that will be called once the AutoSolveButton is clicked
	virtual void AutoSolveButtonClicked() override;

	//------- Left Page --------//
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left00;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left01;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left10;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Left11;


	//------- Right Page --------//
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right00;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right01;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right10;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Right11;
	

	//------- Workspace --------//
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
};
