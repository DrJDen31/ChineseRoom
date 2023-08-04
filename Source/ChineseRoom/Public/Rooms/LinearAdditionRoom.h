// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TheRoomMenu.h"
#include "LinearAdditionRoom.generated.h"

/**
 * 
 */
UCLASS()
class CHINESEROOM_API ULinearAdditionRoom : public UTheRoomMenu
{
	GENERATED_BODY()
	
public:
	virtual void MenuSetup(TSubclassOf<UChineseRoomLevel> InLevel) override;

protected:
	virtual void SetUpImageArrays() override;
	virtual void SetImageArray(TArray<FSpecialCharacterImageRow> InImageArray, FWindow InWindow) override;

	//Function that will be called once the AutoSolveButton is clicked
	virtual void AutoSolveButtonClicked() override;

	// Checks if two windows are the same
	int DoesRuleApply();

	void ApplyRule(int Rule);

	void ShiftWorkspace();

	//Function that will be called once the PrevPageButton is clicked
	virtual void PrevPageButtonClicked() override;

	//Function that will be called once the NextPageButton is clicked
	virtual void NextPageButtonClicked() override;

	//Function that will be called once the PrevBookButton is clicked
	virtual void PrevBookButtonClicked() override;

	//Function that will be called once the NextBookButton is clicked
	virtual void NextBookButtonClicked() override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void RuleHasOptions(bool Options);

	bool DoesRuleHaveOptions();

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
	
	// Extra - testing
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
};
