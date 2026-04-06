// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacingGWheelRear.h"
#include "UObject/ConstructorHelpers.h"

URacingGWheelRear::URacingGWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}