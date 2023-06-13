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
	Circle UMETA(DisplayName = "Circle"),
	Triangle UMETA(DisplayName = "Triangle"),
	Square UMETA(DisplayName = "Square"),
};

USTRUCT(BlueprintType)
struct FSpecialCharacterColumn
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<EShapeSpecialCharacter> Row;
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
	TArray<FSpecialCharacterColumn> Contents;
};

USTRUCT(BlueprintType)
struct FPage
{
	GENERATED_USTRUCT_BODY();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWindow LeftPage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FWindow RightPage;
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