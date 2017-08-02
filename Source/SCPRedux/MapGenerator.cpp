

#include "MapGenerator.h"

TArray<FRoomSpawn> UMapGenerator::GenerateMap()
{
	ParseDataTable();

	return TArray<FRoomSpawn>();
}

void UMapGenerator::ParseDataTable()
{
	UEnum* roomsEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERooms"), true);

	// init maps and arrays

	for (auto it : RoomsTable->RowMap)
	{
		FRoomAttributes* rowData = (FRoomAttributes*)(it.Value);
		ERooms type = (ERooms)roomsEnum->GetValueByNameString(it.Key.ToString());

		ClassRelation.Add(type, rowData->Room);

		switch (rowData->Zone)
		{
		case ERoomZones::Light:
			if (rowData->Chance > 0.0f)
			{
				switch (rowData->Shape)
				{
				case ERoomShapes::Endcap:
					LCZGenericEndcaps.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Hallway:
					LCZGenericHallways.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Corner:
					LCZGenericCorners.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Threeway:
					LCZGenericThreeways.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Fourway:
					LCZGenericFourways.Add(type, rowData->Chance);
					break;
				}
			}
			else
			{
				switch (rowData->Shape)
				{
				case ERoomShapes::Endcap:
					LCZUniqueEndcaps.Add(type);
					break;
				case ERoomShapes::Hallway:
					LCZUniqueHallways.Add(type);
					break;
				case ERoomShapes::Corner:
					LCZUniqueCorners.Add(type);
					break;
				case ERoomShapes::Threeway:
					LCZUniqueThreeways.Add(type);
					break;
				case ERoomShapes::Fourway:
					LCZUniqueFourways.Add(type);
					break;
				}
			}
			break;

		case ERoomZones::Heavy:
			if (rowData->Chance > 0.0f)
			{
				switch (rowData->Shape)
				{
				case ERoomShapes::Endcap:
					HCZGenericEndcaps.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Hallway:
					HCZGenericHallways.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Corner:
					HCZGenericCorners.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Threeway:
					HCZGenericThreeways.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Fourway:
					HCZGenericFourways.Add(type, rowData->Chance);
					break;
				}
			}
			else
			{
				switch (rowData->Shape)
				{
				case ERoomShapes::Endcap:
					HCZUniqueEndcaps.Add(type);
					break;
				case ERoomShapes::Hallway:
					HCZUniqueHallways.Add(type);
					break;
				case ERoomShapes::Corner:
					HCZUniqueCorners.Add(type);
					break;
				case ERoomShapes::Threeway:
					HCZUniqueThreeways.Add(type);
					break;
				case ERoomShapes::Fourway:
					HCZUniqueFourways.Add(type);
					break;
				}
			}
			break;

		case ERoomZones::Entrance:
			if (rowData->Chance > 0.0f)
			{
				switch (rowData->Shape)
				{
				case ERoomShapes::Endcap:
					EZGenericEndcaps.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Hallway:
					EZGenericHallways.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Corner:
					EZGenericCorners.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Threeway:
					EZGenericThreeways.Add(type, rowData->Chance);
					break;
				case ERoomShapes::Fourway:
					EZGenericFourways.Add(type, rowData->Chance);
					break;
				}
			}
			else
			{
				switch (rowData->Shape)
				{
				case ERoomShapes::Endcap:
					EZUniqueEndcaps.Add(type);
					break;
				case ERoomShapes::Hallway:
					EZUniqueHallways.Add(type);
					break;
				case ERoomShapes::Corner:
					EZUniqueCorners.Add(type);
					break;
				case ERoomShapes::Threeway:
					EZUniqueThreeways.Add(type);
					break;
				case ERoomShapes::Fourway:
					EZUniqueFourways.Add(type);
					break;
				}
			}
			break;
		}
	}
}
