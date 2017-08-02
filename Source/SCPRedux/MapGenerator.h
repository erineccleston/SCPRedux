

#pragma once

#include "Enumerations.h"
#include "Structures.h"
#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGenerator.generated.h"

/**
 * 
 */
UCLASS()
class SCPREDUX_API UMapGenerator : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	TArray<FRoomSpawn> GenerateMap();

private:
	void ParseDataTable();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	FRandomStream Rand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (ExposeOnSpawn))
	UDataTable* RoomsTable;

private:
	TMap<ERooms, TSubclassOf<AActor>> ClassRelation;

	TMap<ERooms, float> LCZGenericEndcaps;
	TMap<ERooms, float> LCZGenericHallways;
	TMap<ERooms, float> LCZGenericCorners;
	TMap<ERooms, float> LCZGenericThreeways;
	TMap<ERooms, float> LCZGenericFourways;

	TArray<ERooms> LCZUniqueEndcaps;
	TArray<ERooms> LCZUniqueHallways;
	TArray<ERooms> LCZUniqueCorners;
	TArray<ERooms> LCZUniqueThreeways;
	TArray<ERooms> LCZUniqueFourways;

	TMap<ERooms, float> HCZGenericEndcaps;
	TMap<ERooms, float> HCZGenericHallways;
	TMap<ERooms, float> HCZGenericCorners;
	TMap<ERooms, float> HCZGenericThreeways;
	TMap<ERooms, float> HCZGenericFourways;

	TArray<ERooms> HCZUniqueEndcaps;
	TArray<ERooms> HCZUniqueHallways;
	TArray<ERooms> HCZUniqueCorners;
	TArray<ERooms> HCZUniqueThreeways;
	TArray<ERooms> HCZUniqueFourways;

	TMap<ERooms, float> EZGenericEndcaps;
	TMap<ERooms, float> EZGenericHallways;
	TMap<ERooms, float> EZGenericCorners;
	TMap<ERooms, float> EZGenericThreeways;
	TMap<ERooms, float> EZGenericFourways;

	TArray<ERooms> EZUniqueEndcaps;
	TArray<ERooms> EZUniqueHallways;
	TArray<ERooms> EZUniqueCorners;
	TArray<ERooms> EZUniqueThreeways;
	TArray<ERooms> EZUniqueFourways;
};
