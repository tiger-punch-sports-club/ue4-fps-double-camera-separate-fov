// Copyright Epic Games, Inc. All Rights Reserved.

#include "FovTutorialGameMode.h"
#include "FovTutorialHUD.h"
#include "FovTutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFovTutorialGameMode::AFovTutorialGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFovTutorialHUD::StaticClass();
}
