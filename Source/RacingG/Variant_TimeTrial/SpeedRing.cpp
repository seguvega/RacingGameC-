// Copyright Epic Games, Inc. All Rights Reserved.

#include "SpeedRing.h"

#include "Components/BoxComponent.h"
#include "Engine/Engine.h"
#include "RacingGPawn.h"
#include "ChaosWheeledVehicleMovementComponent.h"

ASpeedRing::ASpeedRing()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = Trigger;

	Trigger->SetBoxExtent(FVector(150.0f));
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionObjectType(ECC_WorldDynamic);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Trigger->SetGenerateOverlapEvents(true);
	Trigger->bHiddenInGame = true;
}

void ASpeedRing::BeginPlay()
{
	Super::BeginPlay();

	if (Trigger)
	{
		Trigger->OnComponentBeginOverlap.AddDynamic(this, &ASpeedRing::OnTriggerBeginOverlap);
	}
}

void ASpeedRing::OnTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	ARacingGPawn* VehiclePawn = Cast<ARacingGPawn>(OtherActor);
	if (!VehiclePawn)
	{
		return;
	}

	UPrimitiveComponent* VehicleMesh = Cast<UPrimitiveComponent>(VehiclePawn->GetMesh());
	if (!VehicleMesh || !VehicleMesh->IsSimulatingPhysics())
	{
		return;
	}

	const float BoostMultiplier = FMath::Max(1.0f, 1.0f + SpeedBoostPercent);

	// Prefer the movement component's forward speed if present.
	float CurrentForwardSpeed = 0.0f;
	if (UChaosWheeledVehicleMovementComponent* MoveComp = VehiclePawn->GetChaosVehicleMovement())
	{
		CurrentForwardSpeed = MoveComp->GetForwardSpeed(); // cm/s
	}
	else
	{
		CurrentForwardSpeed = FVector::DotProduct(VehicleMesh->GetComponentVelocity(), VehiclePawn->GetActorForwardVector());
	}

	if (FMath::Abs(CurrentForwardSpeed) < MinSpeedToBoost)
	{
		return;
	}

	const FVector Forward = VehiclePawn->GetActorForwardVector().GetSafeNormal();
	const float NewSpeed = CurrentForwardSpeed * BoostMultiplier;

	// Preserve the current vertical velocity component to avoid weird hops.
	const FVector CurrentVel = VehicleMesh->GetComponentVelocity();
	const FVector NewVel = (Forward * NewSpeed) + (FVector::UpVector * CurrentVel.Z);
	VehicleMesh->SetPhysicsLinearVelocity(NewVel, false);

	if (GEngine)
	{
		const float Percent = SpeedBoostPercent * 100.0f;
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.5f,
			FColor::Green,
			FString::Printf(TEXT("Speed Ring! +%.0f%% speed"), Percent));
	}
}
