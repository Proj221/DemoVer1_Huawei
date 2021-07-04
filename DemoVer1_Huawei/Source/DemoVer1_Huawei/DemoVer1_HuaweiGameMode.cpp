// Copyright Epic Games, Inc. All Rights Reserved.

#include "DemoVer1_HuaweiGameMode.h"
#include "DemoVer1_HuaweiPlayerController.h"
#include "DemoVer1_HuaweiCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADemoVer1_HuaweiGameMode::ADemoVer1_HuaweiGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ADemoVer1_HuaweiPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDownCPP/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}