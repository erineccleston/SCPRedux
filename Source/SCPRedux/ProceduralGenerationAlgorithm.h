// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralGenerationAlgorithm.generated.h"

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	None,// 	UMETA(DisplayName = "Dance"),
	GenericEndcap,// 	UMETA(DisplayName = "Rain"),
	GenericHallway,//	UMETA(DisplayName = "Song")
	GenericCorner,
	GenericThreeway,
	GenericFourway,
	ZoneTransitionLight,
	ZoneTransitionEntrance,
	Room173
};

USTRUCT(BlueprintType)
struct FTileStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Struct")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Struct")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Struct")
	ERoomType Type;
};

/**
 * Generates locations for game tiles. Assumes map size and tile size.
 */
UCLASS()
class SCPREDUX_API UProceduralGenerationAlgorithm : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FTileStruct> GenerateTiles();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	FRandomStream Rand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<ERoomType> RequiredEndcaps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<ERoomType> RequiredHallways;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<ERoomType> RequiredCorners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<ERoomType> Required3Ways;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<ERoomType> Required4Ways;

private:
	TArray<TArray<ERoomType>> map;

	void GenerateBaseMap();
	void AddEndcaps();
	void ReplaceGenerics();

	TArray<FTileStruct> PackageMap();

	int32 Max(TArray<int32> arr);
	int32 Min(TArray<int32> arr);

	TArray<FVector2D> FindAll(ERoomType type);
	FVector CoordToFVector(int32 x, int32 y);
};
