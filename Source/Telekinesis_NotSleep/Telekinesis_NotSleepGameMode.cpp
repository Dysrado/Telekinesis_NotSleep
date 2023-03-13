// Copyright Epic Games, Inc. All Rights Reserved.

#include "Telekinesis_NotSleepGameMode.h"
#include "Telekinesis_NotSleepCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATelekinesis_NotSleepGameMode::ATelekinesis_NotSleepGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
