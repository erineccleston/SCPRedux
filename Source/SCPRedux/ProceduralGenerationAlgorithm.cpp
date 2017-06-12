// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralGenerationAlgorithm.h"

TArray<FTileStruct> UProceduralGenerationAlgorithm::GenerateTiles()
{
	GenerateBaseMap();
	AddEndcaps();
	DetermineRotations();
	ReplaceGenerics();

	return PackageMap(); 
}

TArray<FTileStruct> UProceduralGenerationAlgorithm::PackageMap()
{
	TArray<FTileStruct> arr;

	for (int32 x = 0; x < 15; x++)
	{
		for (int32 y = 0; y < 6; y++)
		{
			if (map[x][y] != ERoomType::None)
			{
				FTileStruct tile;
				tile.Location = CoordToFVector(x, y);
				tile.Rotation = rotMap[x][y];
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
	rotMap.SetNum(15);
	for (int32 x = 0; x < 15; x++)
	{
		map[x].SetNum(6);
		rotMap[x].SetNum(6);
		for (int32 y = 0; y < 6; y++)
		{
			map[x][y] = ERoomType::None;
			rotMap[x][y] = FRotator(0, 0, 0);
		}
	}

	map[7][5] = ERoomType::Room173;

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
		map[ztOffset + num][0] = ERoomType::ZoneTransitionLight;
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
		map[shOffset + num][2] = ERoomType::GenericHallway;
		map[shOffset + num][3] = ERoomType::GenericHallway;
	}

	// Generate top hall tiles

	int32 thStart = Rand.RandRange(0, Min(ztPositions) + ztOffset);
	int32 thEnd = Rand.RandRange(Max(ztPositions) + ztOffset, 14);

	for (int32 i = thStart; i <= thEnd; i++)
	{
		ERoomType tileType;

		if (i == thStart || i == thEnd)
		{
			if (map[i][0] == ERoomType::None && map[i][2] == ERoomType::None)
				tileType = ERoomType::GenericEndcap;
			else if (map[i][0] != ERoomType::None && map[i][2] != ERoomType::None)
				tileType = ERoomType::GenericThreeway;
			else
				tileType = ERoomType::GenericCorner;
		}
		else
		{
			if (map[i][0] == ERoomType::None && map[i][2] == ERoomType::None)
				tileType = ERoomType::GenericHallway;
			else if (map[i][0] != ERoomType::None && map[i][2] != ERoomType::None)
				tileType = ERoomType::GenericFourway;
			else
				tileType = ERoomType::GenericThreeway;
		}

		map[i][1] = tileType;
	}

	// Generate bottom hall tiles

	int32 bhStart = Rand.RandRange(0, Min(shPositions) + shOffset);
	int32 bhEnd = Rand.RandRange(Max(shPositions) + shOffset, 14);

	for (int32 i = bhStart; i <= bhEnd; i++)
	{
		ERoomType tileType;

		if (i == bhStart || i == bhEnd)
		{
			if (map[i][3] == ERoomType::None && map[i][5] == ERoomType::None)
				tileType = ERoomType::GenericEndcap;
			else if (map[i][3] != ERoomType::None && map[i][5] != ERoomType::None)
				tileType = ERoomType::GenericThreeway;
			else
				tileType = ERoomType::GenericCorner;
		}
		else
		{
			if (map[i][3] == ERoomType::None && map[i][5] == ERoomType::None)
				tileType = ERoomType::GenericHallway;
			else if (map[i][3] != ERoomType::None && map[i][5] != ERoomType::None)
				tileType = ERoomType::GenericFourway;
			else
				tileType = ERoomType::GenericThreeway;
		}

		map[i][4] = tileType;
	}
}

void UProceduralGenerationAlgorithm::AddEndcaps()
{
	TArray<FVector2D> endcaps   = FindAll(ERoomType::GenericEndcap);
	TArray<FVector2D> halls     = FindAll(ERoomType::GenericHallway);
	TArray<FVector2D> threeways = FindAll(ERoomType::GenericThreeway);

	int32 timeout = 0;
	for (int32 i = 0; i < RequiredEndcaps.Num() - endcaps.Num(); i++)
	{
		bool foundTile = false;
		int32 superx = 0;
		int32 supery = 0;
		while (!foundTile && timeout < 1000)
		{
			timeout++;

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
			if (map[x][y] == ERoomType::None)
				cont = true;

			if (cont)
			{
				int32 spaces = 0;

				if (!(x == 0 || y == 0))
				{
					if (map[x - 1][y - 1] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 0))
				{
					if (map[x - 1][y] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 0 || y == 5))
				{
					if (map[x - 1][y + 1] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (!(y == 0))
				{
					if (map[x][y - 1] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (!(y == 5))
				{
					if (map[x][y + 1] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 14 || y == 0))
				{
					if (map[x + 1][y - 1] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 14))
				{
					if (map[x + 1][y] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (!(x == 14 || y == 5))
				{
					if (map[x + 1][y + 1] == ERoomType::None)
						spaces++;
				}
				else { spaces++; }

				if (spaces == 5 && !(y == 0 || y == 5))
				{
					map[x][y] = ERoomType::GenericEndcap;
					foundTile = true;
					timeout = 0;
				}
			}
		}

		if (foundTile)
		{
			if (map[superx][supery] == ERoomType::GenericHallway)
			{
				map[superx][supery] = ERoomType::GenericThreeway;
			}
			else
			{
				map[superx][supery] = ERoomType::GenericFourway;
			}
		}
	}
}

void UProceduralGenerationAlgorithm::DetermineRotations()
{
	FRotator up(0, 0, 0);
	FRotator down(0, 180, 0);
	FRotator left(0, -90, 0);
	FRotator right(0, 90, 0);

	for (int32 x = 0; x < 15; x++)
	{
		for (int32 y = 0; y < 6; y++)
		{
			FRotator local;

			switch (map[x][y])
			{
			case ERoomType::GenericEndcap:
				if (x < 14 && map[x + 1][y] != ERoomType::None)
					local = right;
				else if (x > 0 && map[x - 1][y] != ERoomType::None)
					local = left;
				else if (y < 5 && map[x][y + 1] != ERoomType::None)
					local = down;
				else if (y > 0 && map[x][y - 1] != ERoomType::None)
					local = up;
				break;
			case ERoomType::GenericHallway:
				if (map[x + 1][y] == ERoomType::None)
					local = FRotator(0, Rand.RandRange(0, 1) * 180, 0);
				else
					local = FRotator(0, 90 + Rand.RandRange(0, 1) * 180, 0);
				break;
			case ERoomType::GenericCorner:
				if (x < 14 && map[x + 1][y] != ERoomType::None)
				{
					if (y < 5 && map[x][y + 1] != ERoomType::None)
						local = down;
					else
						local = right;
				}
				else
				{
					if (y > 0 && map[x][y - 1] != ERoomType::None)
						local = up;
					else
						local = left;
				}
				break;
			case ERoomType::GenericThreeway:
				if (map[x + 1][y] == ERoomType::None)
					local = left;
				else if (map[x - 1][y] == ERoomType::None)
					local = right;
				else if(map[x][y + 1] == ERoomType::None)
					local = up;
				else if(map[x][y - 1] == ERoomType::None)
					local = down;
				break;
			case ERoomType::GenericFourway:
				local = FRotator(0, Rand.RandRange(0, 3) * 90, 0);
				break;
			default:
				local = up;
				break;
			}

			rotMap[x][y] = local;
		}
	}
}

void UProceduralGenerationAlgorithm::ReplaceGenerics()
{
	TArray<FVector2D> endcaps   = FindAll(ERoomType::GenericEndcap);
	TArray<FVector2D> halls     = FindAll(ERoomType::GenericHallway);
	TArray<FVector2D> corners   = FindAll(ERoomType::GenericCorner);
	TArray<FVector2D> threeways = FindAll(ERoomType::GenericThreeway);
	TArray<FVector2D> fourways  = FindAll(ERoomType::GenericFourway);

	for (auto& room : RequiredEndcaps)
	{
		if (endcaps.Num() > 0)
		{
			int32 index = Rand.RandRange(0, endcaps.Num() - 1);
			map[endcaps[index].X][endcaps[index].Y] = room;
			endcaps.RemoveAt(index);
		}
	}

	for (auto& room : RequiredHallways)
	{
		if (halls.Num() > 0)
		{
			int32 index = Rand.RandRange(0, halls.Num() - 1);
			map[halls[index].X][halls[index].Y] = room;
			halls.RemoveAt(index);
		}
	}

	for (auto& room : RequiredCorners)
	{
		if (corners.Num() > 0)
		{
			int32 index = Rand.RandRange(0, corners.Num() - 1);
			map[corners[index].X][corners[index].Y] = room;
			corners.RemoveAt(index);
		}
	}

	for (auto& room : RequiredThreeways)
	{
		if (threeways.Num() > 0)
		{
			int32 index = Rand.RandRange(0, threeways.Num() - 1);
			map[threeways[index].X][threeways[index].Y] = room;
			threeways.RemoveAt(index);
		}
	}

	for (auto& room : RequiredFourways)
	{
		if (fourways.Num() > 0)
		{
			int32 index = Rand.RandRange(0, fourways.Num() - 1);
			map[fourways[index].X][fourways[index].Y] = room;
			fourways.RemoveAt(index);
		}
	}
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

TArray<FVector2D> UProceduralGenerationAlgorithm::FindAll(ERoomType type)
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
	float vecx = (x - 7) * 2050;
	float vecy = (y - 4) * 2050;
	return FVector(vecx, vecy, 0.0f);
}
