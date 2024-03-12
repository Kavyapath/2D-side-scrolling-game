// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tesst_AlienDogGameMode.h"
#include "Tesst_AlienDogCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATesst_AlienDogGameMode::ATesst_AlienDogGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

