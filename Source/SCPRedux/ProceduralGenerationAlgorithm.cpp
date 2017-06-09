// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralGenerationAlgorithm.h"

void UProceduralGenerationAlgorithm::Init()
{
	RequiredEndcaps  = { 914, 372, 205, 178, 10001 };
	RequiredHallways = { 012, 970, 1123, 1499, 20001, 20002, 20003, 20004 };
	RequiredCorners  = { 1162 };
	Required3Ways    = { 939 };
	Required4Ways    = { };
}

TArray<FTileStruct> UProceduralGenerationAlgorithm::GenerateTiles(int32 Seed)
{ 
	Rand = FRandomStream(Seed);

	Init();

	return TArray<FTileStruct>(); 
}

int32** UProceduralGenerationAlgorithm::GenArray()
{
	//fill with 0

	// I forgot to consider corners D:

	return nullptr;
}

FVector UProceduralGenerationAlgorithm::CoordToFVector(int32 x, int32 y)
{
	float vecx = (x - 7) * 2050;
	float vecy = (y - 4) * 2050;
	return FVector(vecx, vecy, 0.0f);
}
