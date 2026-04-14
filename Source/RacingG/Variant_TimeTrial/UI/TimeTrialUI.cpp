// Copyright Epic Games, Inc. All Rights Reserved.


#include "TimeTrialUI.h"
#include "TimeTrialStartUI.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UTimeTrialUI::NativeConstruct()
{
	Super::NativeConstruct();

	// create the start countdown widget
	if (!StartUIClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("TimeTrialUI: StartUIClass is not set; starting race immediately."));
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UTimeTrialUI::StartRace));
		}
		return;
	}

	UTimeTrialStartUI* StartUI = CreateWidget<UTimeTrialStartUI>(GetOwningPlayer(), StartUIClass);
	if (!StartUI)
	{
		UE_LOG(LogTemp, Error, TEXT("TimeTrialUI: Failed to create StartUI widget (class %s); starting race immediately."), *GetNameSafe(StartUIClass));
		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UTimeTrialUI::StartRace));
		}
		return;
	}

	StartUI->AddToViewport(0);

	// subscribe to the countdown finished delegate
	StartUI->OnCountdownFinished.AddDynamic(this, &UTimeTrialUI::StartRace);

	// start the countdown
	StartUI->StartCountdown();
}

void UTimeTrialUI::UpdateLapCount(int32 Lap, float NewLapStartTime)
{
	// save the new lap start time
	LapStartTime = NewLapStartTime;

	// calculate the lap time
	const float LapTime = NewLapStartTime - LastLapTime;

	// is this the first lap?
	if (Lap > 1)
	{
		// do we have an invalid lap time?
		if (BestLapTime < 0.0f)
		{
			// save the current lap time
			BestLapTime = LapTime;

		} else {

			// not the first lap: do we have a lower lap time?
			if (LapTime < BestLapTime)
			{
				// save the best lap time
				BestLapTime = LapTime;
			}

		}
		
	} else {

		// first lap: save an invalid lap time
		BestLapTime = -1.0f;

	}

	// save the current lap
	CurrentLap = Lap;

	// save the lap start time
	LastLapTime = NewLapStartTime;

	// pass control to BP to update the widgets
	BP_UpdateLaps();
}

void UTimeTrialUI::StartRace()
{
	// broadcast the delegate
	OnRaceStart.Broadcast();
}
