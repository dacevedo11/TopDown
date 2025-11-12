// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "PuzzleRotate.h"
#include "Components/BillboardComponent.h"
#include "PuzzleComponent.h"


// Sets default values
APuzzleRotate::APuzzleRotate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<UBillboardComponent>("Root");

	PuzzleComponent = CreateDefaultSubobject<UPuzzleComponent>("Puzzle");

}

void APuzzleRotate::OnReceiveInput(class AActor* InputActor, bool bValue)
{
	PuzzleComponent->SetOutputActive(bValue);
}

void APuzzleRotate::OnRotateTimer()
{
	if (InputActors.Num() == 0)
	{
		return;
	}
	
	// Call PuzzleReset on the APuzzleRotate’s puzzle component
	PuzzleComponent->PuzzleReset();

	// Call ChangeOutputActor(nullptr) on every InputActor
	ResetInputActorsOutputs();	

	// Call ChangeOutputActor(this) on the InputActor corresponding to the current index
	if (InputActors.IsValidIndex(CurrentIndex))
	{
		AActor* CurrentInputActor = InputActors[CurrentIndex].Get();
		if (CurrentInputActor != nullptr)
		{
			UPuzzleComponent* PuzzleComp = CurrentInputActor->FindComponentByClass<UPuzzleComponent>();
			if (PuzzleComp != nullptr)
			{
				PuzzleComp->ChangeOutputActor(this);
			}
		}
	}

	// Increment the current index, wrapping around back to 0 as needed
	CurrentIndex = (CurrentIndex + 1) % InputActors.Num();
}

// Called when the game starts or when spawned
void APuzzleRotate::BeginPlay()
{
	Super::BeginPlay();

	PuzzleComponent->OnReceiveInput.AddDynamic(this, &APuzzleRotate::OnReceiveInput);

	ResetInputActorsOutputs();

	GetWorldTimerManager().SetTimer(TimerHandle, this, &APuzzleRotate::OnRotateTimer, TimerDuration, true);
}

void APuzzleRotate::ResetInputActorsOutputs()
{
	for (TSoftObjectPtr<AActor> InputActorPtr : InputActors)
	{
		AActor* InputActor = InputActorPtr.Get();
		if (InputActor != nullptr)
		{
			UPuzzleComponent* PuzzleComp = InputActor->FindComponentByClass<UPuzzleComponent>();
			if (PuzzleComp != nullptr)
			{
				PuzzleComp->ChangeOutputActor(nullptr);
			}
		}
	}
}

// Called every frame
void APuzzleRotate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

