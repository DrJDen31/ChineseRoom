// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChineseRoomTypes.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EShapeSpecialCharacter : uint8
{
	None UMETA(DisplayName = "None"),
	Blank UMETA(DisplayName = "Blank"),
	DontCare UMETA(DisplayName = "DontCare"),
	Circle UMETA(DisplayName = "Circle"),
	Triangle UMETA(DisplayName = "Triangle"),
	Square UMETA(DisplayName = "Square"),
	Star UMETA(DisplayName = "Star"),
};

USTRUCT(BlueprintType)
struct FSpecialCharacterRow
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EShapeSpecialCharacter> Row;
};

USTRUCT(BlueprintType)
struct FSpecialCharacterImageRow
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UImage*> Row;
};

USTRUCT(BlueprintType)
struct FWindow
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Width;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Height;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSpecialCharacterRow> Contents;
};

UENUM(BlueprintType)
enum class ESpecialRule : uint8
{
	None UMETA(DisplayName = "None"),
	NextBook UMETA(DisplayName = "NextBook"),
	PrevBook UMETA(DisplayName = "PrevBook"),
	FocusWindowReachedEnd UMETA(DisplayName = "FocusWindowReachedEnd"),
};

USTRUCT(BlueprintType)
struct FPage
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWindow LeftPage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString LeftRuleText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWindow RightPage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString RightRuleText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESpecialRule SpecialRule;
};


USTRUCT(BlueprintType)
struct FBook
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPage> Pages;
};


USTRUCT(BlueprintType)
struct FShelf
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FBook> Books;
};

UCLASS(BlueprintType, Blueprintable)
class URoom : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FShelf Shelf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString HelpText;
};

UCLASS(BlueprintType, Blueprintable)
class UChineseRoomLevel : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<URoom> Room;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWindow StartingWorkspace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWindow DesiredOutput;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWindow FocusWindow;
};

UENUM(BlueprintType)
enum class ESolveState : uint8
{
	None UMETA(DisplayName = "None"),
	Locked UMETA(DisplayName = "Locked"),
	NotStarted UMETA(DisplayName = "NotStarted"),
	InProgress UMETA(DisplayName = "InProgress"),
	Current UMETA(DisplayName = "Current"),
	Solved UMETA(DisplayName = "Solved"),
};

USTRUCT(BlueprintType)
struct FLevelSave
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<UChineseRoomLevel> Level;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ESolveState State;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int LevelNumber;
};
