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
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile Struct")
	int32 Type;
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
	TArray<int32> RequiredEndcaps;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<int32> RequiredHallways;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<int32> RequiredCorners;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<int32> Required3Ways;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<int32> Required4Ways;

private:
	TArray<TArray<int32>> map;

	void AddEndcaps();
	void GenerateBaseMap();
	TArray<FTileStruct> PackageMap();

	int32 Max(TArray<int32> arr);
	int32 Min(TArray<int32> arr);

	TArray<FVector2D> FindAll(int32 type);
	FVector CoordToFVector(int32 x, int32 y);
};
