// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpeedRing.generated.h"

class UBoxComponent;

/**
 * Actor that boosts the vehicle's speed when it passes through.
 * Mesh/visuals are optional; this actor is primarily a trigger volume.
 */
UCLASS()
class RACINGG_API ASpeedRing : public AActor
{
	GENERATED_BODY()

public:
	ASpeedRing();

protected:
	virtual void BeginPlay() override;

protected:
	/** Trigger volume used to detect the vehicle passing through */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpeedRing")
	TObjectPtr<UBoxComponent> Trigger;

	/** Percent speed boost applied on overlap (0.10 = +10%) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpeedRing", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SpeedBoostPercent = 0.10f;

	/** Minimum forward speed (cm/s) required to apply the boost */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpeedRing", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MinSpeedToBoost = 1.0f;

protected:
	UFUNCTION()
	void OnTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
