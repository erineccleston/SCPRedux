// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "SCPReduxGameMode.h"
#include "SCPReduxHUD.h"
#include "SCPReduxCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASCPReduxGameMode::ASCPReduxGameMode()
	: Super()
{
	//// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	//DefaultPawnClass = PlayerPawnClassFinder.Class;

	//// use our custom HUD class
	//HUDClass = ASCPReduxHUD::StaticClass();
}
