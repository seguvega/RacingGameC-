// Copyright Epic Games, Inc. All Rights Reserved.

#include "RacingGWheelFront.h"
#include "UObject/ConstructorHelpers.h"

URacingGWheelFront::URacingGWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}