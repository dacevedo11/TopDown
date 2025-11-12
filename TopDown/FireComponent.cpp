// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "FireComponent.h"
#include "Kismet/GameplayStatics.h"

UFireComponent::UFireComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UFireComponent::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsOnFire)
	{
		UFireComponent* OtherFireComponent = OtherActor->FindComponentByClass<UFireComponent>();

		if (OtherFireComponent != nullptr && !OtherFireComponent->bIsOnFire)
		{
			OtherFireComponent->NativeCatchFire();
		}
	}
}

void UFireComponent::NativeCatchFire()
{
	bIsOnFire = true;
	OnCatchFire.Broadcast();
	UGameplayStatics::SpawnSoundAttached(FireSoundBase, this);
	UGameplayStatics::SpawnEmitterAttached(FireParticleSystem, this);
}

void UFireComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UFireComponent::HandleOnBeginOverlap);

	if (bStartsOnFire)
	{
		NativeCatchFire();
	}
}

void UFireComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnComponentBeginOverlap.RemoveAll(this);
}
