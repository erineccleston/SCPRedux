// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralGenerationAlgorithm.generated.h"

USTRUCT(BlueprintType)
struct FTileStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Struct")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Struct")
	int32 Type;
};

/**
 * Generates locations for game tiles. Assumes 2050 tile size. Does not deal with rotation.
 */
UCLASS()
class SCPREDUX_API UProceduralGenerationAlgorithm : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FTileStruct> GenerateTiles(int32 Seed);

private:
	FRandomStream Rand;

	TArray<TArray<int32>> GenArray();

	void GenerateBaseMap();

	TArray<FTileStruct> PackageMap();

	TArray<TArray<int32>> map;
	
	FVector CoordToFVector(int32 x, int32 y);

	TArray<int32> RequiredEndcaps, RequiredHallways, RequiredCorners, Required3Ways, Required4Ways;

	void Init();

	int32 Max(TArray<int32> arr);

	int32 Min(TArray<int32> arr);

	class OrderedPair
	{
	public:
		OrderedPair(int32 inx, int32 iny) { x = inx; y = iny; }
		int32 x, y;
	};
};
