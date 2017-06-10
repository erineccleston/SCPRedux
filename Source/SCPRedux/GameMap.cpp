// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMap.h"

GameMap::GameMap(int32 width, int32 height)
{

}

GameMap::~GameMap()
{
}

int32 GameMap::Get(int32 x, int32 y)
{
	return Map[x][y];
}
