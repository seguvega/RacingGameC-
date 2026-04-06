// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RacingGPawn.h"
#include "RacingGSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class ARacingGSportsCar : public ARacingGPawn
{
	GENERATED_BODY()
	
public:

	ARacingGSportsCar();
};
