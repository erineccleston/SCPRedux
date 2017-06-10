// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralGenerationAlgorithm.h"

void UProceduralGenerationAlgorithm::Init()
{
	RequiredEndcaps  = { 914, 372, 205, 178, 10001 };
	RequiredHallways = { 012, 970, 1123, 1499, 20001, 20002, 20003, 20004 };
	RequiredCorners  = { 1162 };
	Required3Ways    = { 939 };
	Required4Ways    = { };
	// Other tiles: 1 = endcap, 2 = hallway, 3 = 3way, 4 = 4way, 5 = corner, 6 = zone transition, 173 = 173's room
}

TArray<FTileStruct> UProceduralGenerationAlgorithm::GenerateTiles(int32 Seed)
{ 
	Rand = FRandomStream(Seed);

	//Init();
	GenerateBaseMap();

	return PackageMap(); 
}

TArray<FTileStruct> UProceduralGenerationAlgorithm::PackageMap()
{
	TArray<FTileStruct> arr;

	for (int32 x = 0; x < 15; x++)
	{
		for (int32 y = 0; y < 6; y++)
		{
			if (map[x][y] != 0)
			{
				FTileStruct tile;
				tile.Location = CoordToFVector(x, y);
				tile.Type = map[x][y];
				arr.Push(tile);
			}
		}
	}

	return arr;
}

void UProceduralGenerationAlgorithm::GenerateBaseMap()
{
	map.SetNum(15);
	for (int32 x = 0; x < 15; x++)
	{
		map[x].SetNum(6);
		for (int32 y = 0; y < 6; y++)
		{
			map[x][y] = 0;
		}
	}

	map[7][5] = 173;

	// Generate zone transition tiles

	int32 zoneTransCount = Rand.RandRange(1, 3) + 2;

	TArray<int32> ztPositions;
	for (int32 i = 0; i < zoneTransCount; i++)
	{
		int32 toAdd = Rand.RandRange(0, 14);
		ztPositions.Push(toAdd);

		for (auto& num : ztPositions)
		{
			if ((toAdd + 1 == num) || (toAdd - 1 == num))
			{
				i--;
				ztPositions.Pop();
				break;
			}
		}
	}

	int32 ztOffset = Rand.RandRange(0, 14 - Max(ztPositions));

	for (auto& num : ztPositions)
	{
		map[ztOffset + num][0] = 6;
	}

	// Generate side hall tiles

	int32 sideHallCount = Rand.RandRange(1, 3) + Rand.RandRange(1, 3) - 1;

	TArray<int32> shPositions;
	for (int32 i = 0; i < sideHallCount; i++)
	{
		int32 toAdd = Rand.RandRange(1, 13);
		shPositions.Push(toAdd);

		for (auto& num : shPositions)
		{
			if ((toAdd + 1 == num) || (toAdd - 1 == num))
			{
				i--;
				shPositions.Pop();
				break;
			}
		}
	}

	int32 shOffset = Rand.RandRange(0, 13 - Max(shPositions));

	for (auto& num : shPositions)
	{
		map[shOffset + num][2] = 2;
		map[shOffset + num][3] = 2;
	}

	// Generate top hall tiles

	int32 thStart = Rand.RandRange(0, Min(ztPositions) + ztOffset);
	int32 thEnd = Rand.RandRange(Max(ztPositions) + ztOffset, 14);

	for (int32 i = thStart; i <= thEnd; i++)
	{
		int32 tileType;

		if (i == thStart || i == thEnd)
		{
			if (map[i][0] == 0 && map[i][2] == 0)
				tileType = 1;
			else if (map[i][0] != 0 && map[i][2] != 0)
				tileType = 3;
			else
				tileType = 5;
		}
		else
		{
			if (map[i][0] == 0 && map[i][2] == 0)
				tileType = 2;
			else if (map[i][0] != 0 && map[i][2] != 0)
				tileType = 4;
			else
				tileType = 3;
		}

		map[i][1] = tileType;
	}

	// Generate bottom hall tiles

	int32 bhStart = Rand.RandRange(0, Min(shPositions) + shOffset);
	int32 bhEnd = Rand.RandRange(Max(shPositions) + shOffset, 14);

	for (int32 i = bhStart; i <= bhEnd; i++)
	{
		int32 tileType;

		if (i == bhStart || i == bhEnd)
		{
			if (map[i][3] == 0 && map[i][5] == 0)
				tileType = 1;
			else if (map[i][3] != 0 && map[i][5] != 0)
				tileType = 3;
			else
				tileType = 5;
		}
		else
		{
			if (map[i][3] == 0 && map[i][5] == 0)
				tileType = 2;
			else if (map[i][3] != 0 && map[i][5] != 0)
				tileType = 4;
			else
				tileType = 3;
		}

		map[i][4] = tileType;
	}

	UE_LOG(LogTemp, Warning, TEXT("zonetranscount %d ztoffset %d sidehallcount %d shoffset %d thstart %d thend %d bh start %d bhend %d "), zoneTransCount, ztOffset, sideHallCount, shOffset, thStart, thEnd, bhStart, bhEnd)
}

TArray<TArray<int32>> UProceduralGenerationAlgorithm::GenArray()
{
	//TArray<TArray<int32>> map;
	//TArray<OrderedPair> endcaps, halls, corners, threeways, fourways, zonetrans;

	for (int32 x = 0; x < 15; x++)
	{
		for (int32 y = 0; y < 6; y++)
		{
			map[x][y] = 0;
		}
	}

	map[7][5] = 173;

	// Generate zone transition tiles

	int32 zoneTransCount = Rand.RandRange(1, 3) + 2;

	TArray<int32> ztPositions;
	for (int32 i = 0; i < zoneTransCount; i++)
	{
		int32 toAdd = Rand.RandRange(0, 14);
		ztPositions.Push(toAdd);

		for (auto& num : ztPositions)
		{
			if ((toAdd + 1 == num) || (toAdd - 1 == num))
			{
				i--;
				ztPositions.Pop();
				break;
			}
		}
	}

	int32 ztOffset = Rand.RandRange(0, 14 - Max(ztPositions));

	for (auto& num : ztPositions)
	{
		OrderedPair coord(ztOffset + num, 0);
		map[coord.x][coord.y] = 6;
		//zonetrans.Push(coord);
	}

	// Generate side hall tiles

	int32 sideHallCount = Rand.RandRange(1, 3) + Rand.RandRange(1, 3) - 1;

	TArray<int32> shPositions;
	for (int32 i = 0; i < sideHallCount; i++)
	{
		int32 toAdd = Rand.RandRange(1, 13);
		shPositions.Push(toAdd);

		for (auto& num : shPositions)
		{
			if ((toAdd + 1 == num) || (toAdd - 1 == num))
			{
				i--;
				shPositions.Pop();
				break;
			}
		}
	}

	int32 shOffset = Rand.RandRange(0, 13 - Max(shPositions));

	for (auto& num : shPositions)
	{
		OrderedPair coord1(shOffset + num, 2);
		OrderedPair coord2(shOffset + num, 3);
		map[coord1.x][coord1.y] = 2;
		//halls.Push(coord1);
		map[coord2.x][coord2.y] = 2;
		//halls.Push(coord2);
	}

	// Generate top hall tiles

	int32 thStart = Rand.RandRange(0, Min(ztPositions));
	int32 thEnd   = Rand.RandRange(Max(ztPositions), 14);

	for (int32 i = thStart; i <= thEnd; i++)
	{
		int32 tileType;

		if (i == thStart || i == thEnd)
		{
			if (map[i][0] == 0 && map[i][2] == 0)
				tileType = 1;
			else if (map[i][0] != 0 && map[i][2] != 0)
				tileType = 3;
			else
				tileType = 5;
		}
		else
		{
			if (map[i][0] == 0 && map[i][2] == 0)
				tileType = 2;
			else if (map[i][0] != 0 && map[i][2] != 0)
				tileType = 4;
			else
				tileType = 3;
		}

		OrderedPair coord(i, 1);
		map[coord.x][coord.y] = tileType;

		//switch (tileType)
		//{
		//case 1:
		//	endcaps.Push(coord);
		//	break;
		//case 2:
		//	halls.Push(coord);
		//	break;
		//case 3:
		//	threeways.Push(coord);
		//	break;
		//case 4:
		//	fourways.Push(coord);
		//	break;
		//case 5:
		//	corners.Push(coord);
		//	break;
		//}
	}

	// Generate bottom hall tiles

	int32 bhStart = Rand.RandRange(0, Min(shPositions));
	int32 bhEnd = Rand.RandRange(Max(shPositions), 14);

	for (int32 i = bhStart; i <= bhEnd; i++)
	{
		int32 tileType;

		if (i == bhStart || i == bhEnd)
		{
			if (map[i][3] == 0 && map[i][5] == 0)
				tileType = 1;
			else if (map[i][3] != 0 && map[i][5] != 0)
				tileType = 3;
			else
				tileType = 5;
		}
		else
		{
			if (map[i][3] == 0 && map[i][5] == 0)
				tileType = 2;
			else if (map[i][3] != 0 && map[i][5] != 0)
				tileType = 4;
			else
				tileType = 3;
		}

		OrderedPair coord(i, 4);
		map[coord.x][coord.y] = tileType;

		//switch (tileType)
		//{
		//case 1:
		//	endcaps.Push(coord);
		//	break;
		//case 2:
		//	halls.Push(coord);
		//	break;
		//case 3:
		//	threeways.Push(coord);
		//	break;
		//case 4:
		//	fourways.Push(coord);
		//	break;
		//case 5:
		//	corners.Push(coord);
		//	break;
		//}
	}
	/*
	// Create additional endcaps

	for (int32 i = 0; i < RequiredEndcaps.Num() - endcaps.Num(); i++)
	{
		bool foundTile = false;
		int32 superx, supery;
		while (!foundTile)
		{
			int32 randIndex1 = Rand.RandRange(0, halls.Num());
			int32 randIndex2 = Rand.RandRange(0, threeways.Num());
			int32 x, y;

			if (Rand.RandRange(0, 1) == 0)
			{
				x = halls[randIndex1].x;
				y = halls[randIndex1].y;
			}
			else
			{
				x = threeways[randIndex2].x;
				y = threeways[randIndex2].y;
			}

			OrderedPair randTile(x, y);
			superx = x;
			supery = y;

			int32 randDir = Rand.RandRange(0, 3);
			switch (randDir)
			{
			case 0:
				x--;
				break;
			case 1:
				x++;
				break;
			case 2:
				y--;
				break;
			case 3:
				y++;
				break;
			}

			bool cont = false;
			if (map[x][y] == 0)
				cont = true;

			if (cont)
			{
				OrderedPair possibleTile(x, y);

				int32 keys = 0;

				if (map[x + 1][y] == 0)
					keys++;

				if (map[x - 1][y] == 0)
					keys++;

				if (map[x][y + 1] == 0)
					keys++;

				if (map[x][y - 1] == 0)
					keys++;

				if (keys == 3 && !(y == 0 || y == 5))
				{
					map[x][y] = 1;
					foundTile = true;
					endcaps.Push(possibleTile);
				}
			}
		}

		if (map[superx][supery] == 2)
		{
			map[superx][supery] = 3;
			//remove from halls
			//add to threeways
		}
		else
		{
			map[superx][supery] = 4;
			//remove from threeways
			//add to fourways
		}
	}*/

	// Replace generic tiles with required tiles

	//for (auto& num : RequiredEndcaps)
	//{
	//	int32 index = Rand.RandRange(0, endcaps.Num());
	//	map[endcaps[index].x][endcaps[index].y] = num;
	//	endcaps.RemoveAt(index);
	//}

	//for (auto& num : RequiredHallways)
	//{
	//	int32 index = Rand.RandRange(0, halls.Num());
	//	map[halls[index].x][halls[index].y] = num;
	//	halls.RemoveAt(index);
	//}

	return TArray<TArray<int32>>();
	//return nullptr;// map;
}

int32 UProceduralGenerationAlgorithm::Max(TArray<int32> arr)
{
	int32 max = arr[0];
	for (auto& num : arr)
	{
		if (max < num)
			max = num;
	}
	return max;
}

int32 UProceduralGenerationAlgorithm::Min(TArray<int32> arr)
{
	int32 min = arr[0];
	for (auto& num : arr)
	{
		if (min > num)
			min = num;
	}
	return min;
}

FVector UProceduralGenerationAlgorithm::CoordToFVector(int32 x, int32 y)
{
	//float vecx = (x - 7) * 2050;
	//float vecy = (y - 4) * 2050;
	//return FVector(vecx, vecy, 0.0f);
	return FVector(x - 7, y - 4, 0.0f);//
}
