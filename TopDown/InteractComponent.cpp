// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "InteractComponent.h"
#include "InteractSubsystem.h"
#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UInteractComponent::UInteractComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
			Subsystem->AddComponent(this);
	}
}

void UInteractComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->RemoveComponent(this);
	}
}


// Called every frame
void UInteractComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractComponent::NativeInteract()
{
	if (!bInteractionEnabled)
	{
		return;
	}
	
	OnInteract.Broadcast();
}

void UInteractComponent::SetGlowEnabled(bool bEnabled)
{
	if (bGlowEnabled == bEnabled)
	{
		return;
	}

	bGlowEnabled = bEnabled;

	AActor* Owner = GetOwner();
	if (!Owner)
	{
		return;
	}

	UStaticMeshComponent* MeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
	if (!MeshComponent)
	{
		return;
	}

	if (bEnabled)
	{
		FVector GlowVector(1.0f, 1.0f, 1.0f);
		MeshComponent->SetVectorParameterValueOnMaterials(GlowParameterName, GlowVector);
	}
	else
	{
		MeshComponent->SetVectorParameterValueOnMaterials(GlowParameterName, FVector::ZeroVector);
	}
}

