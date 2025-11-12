// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "InteractSubsystem.h"
#include "InteractComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UInteractSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UInteractSubsystem::Deinitialize()
{
	Super::Deinitialize();
	InteractComponents.Empty();
}

void UInteractSubsystem::AddComponent(UInteractComponent* Component)
{
	InteractComponents.Add(Component);	
}

void UInteractSubsystem::RemoveComponent(UInteractComponent* Component)
{
	InteractComponents.Remove(Component);
}

TStatId UInteractSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UInteractSubsystem, STATGROUP_Tickables);
}

void UInteractSubsystem::PerformInteract()
{
	if (UInteractComponent* Component = BestCandidate.Get())
	{
		Component->NativeInteract();
	}
}

void UInteractSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BestCandidate = nullptr;
	
	if (ACharacter* Character = UGameplayStatics::GetPlayerCharacter(this, 0))
	{
		FVector PlayerLocation = Character->GetActorLocation();
		FVector PlayerForward = Character->GetActorForwardVector();

		float BestDotProduct = -1.0f;
		const float MaxInteractionDistance = 200.0f;

		for (const auto& WeakPtr : InteractComponents)
		{
			if (UInteractComponent* Component = WeakPtr.Get())
			{
				if (!Component->bInteractionEnabled)
				{
					continue;
				}
				AActor* ComponentOwner = Component->GetOwner();
				if (!ComponentOwner)
				{
					continue;
				}

				FVector ComponentLocation = ComponentOwner-> GetActorLocation();

				float DistanceToComponent = FVector::Dist(PlayerLocation, ComponentLocation);
				if (DistanceToComponent > MaxInteractionDistance)
				{
					continue;
				}

				FVector ToComponent = ComponentLocation - PlayerLocation;
				FVector ToComponentNormalized = ToComponent.GetSafeNormal();

				float DotProduct = FVector::DotProduct(PlayerForward, ToComponentNormalized);

				if (DotProduct <= 0.0f)
				{
					continue;
				}

				if (DotProduct > BestDotProduct)
				{
					BestDotProduct = DotProduct;
					BestCandidate = Component;
				}
			}
		}
	}

	for (const auto& WeakPtr : InteractComponents)
	{
		if (UInteractComponent* Component = WeakPtr.Get())
		{
			bool bShouldGlow = (Component == BestCandidate.Get());
			Component->SetGlowEnabled(bShouldGlow);
		}
	}
}

bool UInteractSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}
