// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralGenerationAlgorithm.h"

TArray<FTileStruct> UProceduralGenerationAlgorithm::GenerateTiles()
{
	GenerateBaseMap();
	AddEndcaps();

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
				tile.Rotation = FRotator();
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

	//UE_LOG(LogTemp, Warning, TEXT("zonetranscount %d ztoffset %d sidehallcount %d shoffset %d thstart %d thend %d bh start %d bhend %d "), zoneTransCount, ztOffset, sideHallCount, shOffset, thStart, thEnd, bhStart, bhEnd)
}

void UProceduralGenerationAlgorithm::AddEndcaps()
{
	TArray<FVector2D> endcaps   = FindAll(1);
	TArray<FVector2D> halls     = FindAll(2);
	TArray<FVector2D> threeways = FindAll(3);

	// Create additional endcaps

	for (int32 i = 0; i < RequiredEndcaps.Num() - endcaps.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Iteration %d"), i)

		bool foundTile = false;
		int32 superx = 0;
		int32 supery = 0;
		while (!foundTile)
		{
			int32 randIndex1 = Rand.RandRange(0, halls.Num() - 1);
			int32 randIndex2 = Rand.RandRange(0, threeways.Num() - 1);
			int32 x, y;

			if (Rand.RandRange(0, 1) == 0)
			{
				x = halls[randIndex1].X;
				y = halls[randIndex1].Y;
			}
			else
			{
				x = threeways[randIndex2].X;
				y = threeways[randIndex2].Y;
			}

			//OrderedPair randTile(x, y);
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
				//OrderedPair possibleTile(x, y);

				int32 spaces = 0;

				if (!(x == 0 || y == 0))
				{
					if (map[x - 1][y - 1] == 0)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 0))
				{
					if (map[x - 1][y] == 0)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 0 || y == 5))
				{
					if (map[x - 1][y + 1] == 0)
						spaces++;
				}
				else { spaces++; }

				if (!(y == 0))
				{
					if (map[x][y - 1] == 0)
						spaces++;
				}
				else { spaces++; }

				if (!(y == 5))
				{
					if (map[x][y + 1] == 0)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 14 || y == 0))
				{
					if (map[x + 1][y - 1] == 0)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 14))
				{
					if (map[x + 1][y] == 0)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 14 || y == 5))
				{
					if (map[x + 1][y + 1] == 0)
						spaces++;
				}
				else { spaces++; }

				if (spaces == 5 && !(y == 0 || y == 5))
				{
					map[x][y] = 1;
					foundTile = true;
					//endcaps.Push(possibleTile);
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
	}

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

TArray<FVector2D> UProceduralGenerationAlgorithm::FindAll(int32 type)
{
	TArray<FVector2D> arr;

	for (int32 x = 0; x < 15; x++)
	{
		for (int32 y = 0; y < 6; y++)
		{
			if (map[x][y] == type)
			{
				FVector2D pair(x, y);
				arr.Push(pair);
			}
		}
	}

	return arr;
}

FVector UProceduralGenerationAlgorithm::CoordToFVector(int32 x, int32 y)
{
	//float vecx = (x - 7) * 2050;
	//float vecy = (y - 4) * 2050;
	//return FVector(vecx, vecy, 0.0f);
	return FVector(x - 7, y - 4, 0.0f);//
}
