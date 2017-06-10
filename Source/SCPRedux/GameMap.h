// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class SCPREDUX_API GameMap
{
public:
	GameMap(int32 width, int32 height);
	~GameMap();

	int32 Get(int32 x, int32 y);
	void Set(int32 x, int32 y, int32 val);
	TArray<FVector2D> Find(int32 val);

private:
	TArray<TArray<int32>> Map;
};
