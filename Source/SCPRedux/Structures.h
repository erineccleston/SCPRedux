

#pragma once

#include "Enumerations.h"
#include "Engine/DataTable.h"

#include "CoreMinimal.h"
#include "Structures.generated.h"

USTRUCT(BlueprintType)
struct FRoomSpawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Room;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;
};

USTRUCT(BlueprintType)
struct FRoomAttributes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> Room;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomShapes Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERoomZones Zone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Chance;
};
