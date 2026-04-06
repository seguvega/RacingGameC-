// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacingGGameMode.h"
#include "RacingGPlayerController.h"

ARacingGGameMode::ARacingGGameMode()
{
	PlayerControllerClass = ARacingGPlayerController::StaticClass();
}
