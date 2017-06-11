// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProceduralGenerationAlgorithm.generated.h"

UENUM(BlueprintType)
enum class ERoomType : uint8
{
	None,
	GenericEndcap,
	GenericHallway,
	GenericCorner,
	GenericThreeway,
	GenericFourway,
	ZoneTransitionLight,
	ZoneTransitionEntrance,
	Room173,
	End178,
	End205,
	End372,
	End914,
	EndArchives,
	Hall012,
	Hall970,
	Hall1123,
	Hall1499,
	HallStoreroom,
	HallTesting,
	HallSurveillance,
	HallSCPs,
	Corner1162,
	TW939
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

/*
 * Generates locations for game tiles. Assumes map dimensions and tile size. Limitation: Won't place required tiles if there's no generic for replacement, so order of the input arrays matters. Array elements at the end will be ignored if there's no room. This affects corner tiles the most.
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
	TArray<ERoomType> RequiredThreeways;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	TArray<ERoomType> RequiredFourways;

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
