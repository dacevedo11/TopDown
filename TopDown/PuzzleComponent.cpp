// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "PuzzleComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UPuzzleComponent::UPuzzleComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UPuzzleComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UPuzzleComponent::SetOutputActive(bool bNewOutputActive)
{
	// Immediately return if the bNewOutputActive is the same as the current bIsOutputActive
	if (bNewOutputActive == bIsOutputActive)
	{
		return;
	}
	
	// If it’s different, set bIsOutputActive to the new value
	bIsOutputActive = bNewOutputActive;

	// Resolve OutputActor to OutActor
	AActor* OutActor = OutputActor.Get();
	if (OutActor != nullptr)
	{
		UPuzzleComponent* PuzzleComp = OutActor->FindComponentByClass<UPuzzleComponent>();
		if (PuzzleComp != nullptr)
		{
			PuzzleComp->NativeReceiveInput(GetOwner(), bNewOutputActive);
		}
	}
}

void UPuzzleComponent::NativeReceiveInput(TObjectPtr<AActor> Sender, bool bIncomingValue)
{
	OnReceiveInput.Broadcast(Sender, bIncomingValue);
}

void UPuzzleComponent::PuzzleReset()
{
	SetOutputActive(false);
	OnPuzzleReset.Broadcast();
}

void UPuzzleComponent::DebugDrawOutput()
{
	if (IsRegistered())
	{
		DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), GetOwner()->GetName(), nullptr, FColor::White, 0.1f);
	}

	
#if ENABLE_DRAW_DEBUG
	if (AActor* OutActor = OutputActor.Get())
	{
		const FColor DrawColor = bIsOutputActive ? FColor::Green : FColor::Black;
		DrawDebugDirectionalArrow(GetWorld(), GetOwner()->GetActorLocation(), OutActor->GetActorLocation(), 150.0f, DrawColor);
	}
#endif
}

void UPuzzleComponent::ChangeOutputActor(AActor* NewOutputActor)
{
	OutputActor = NewOutputActor;
	if (OutputActor != nullptr && bIsOutputActive == true)
	{
		AActor* OutActor = OutputActor.Get();
		if (OutActor != nullptr)
		{
			UPuzzleComponent* PuzzleComp = OutActor->FindComponentByClass<UPuzzleComponent>();
			if (PuzzleComp != nullptr)
			{
				PuzzleComp->NativeReceiveInput(GetOwner(), bIsOutputActive);
			}
		}
	}
}




