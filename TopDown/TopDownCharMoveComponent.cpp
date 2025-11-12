// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "TopDownCharMoveComponent.h"
#include "TopDownCharacter.h"
#include "GameplayTagContainer.h"

void UTopDownCharMoveComponent::BeginPlay()
{
	Super::BeginPlay();

	TopDownChar = GetOwner<ATopDownCharacter>();
}

float UTopDownCharMoveComponent::GetMaxSpeed() const
{
	const FGameplayTag ImmobileTag = FGameplayTag::RequestGameplayTag("Debuff.Immobile", false);
	if (TopDownChar->GetAbilitySystemComponent()->HasMatchingGameplayTag(ImmobileTag))
	{
		return 0.0f;
	}

	return TopDownChar->DefaultAttributeSet->GetMoveSpeed();
}

FRotator UTopDownCharMoveComponent::ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime,
	FRotator& DeltaRotation) const
{
	const FGameplayTag ImmobileTag = FGameplayTag::RequestGameplayTag("Debuff.Immobile", false);
	if (!TopDownChar->GetAbilitySystemComponent()->HasMatchingGameplayTag(ImmobileTag))
	{
		return Super::ComputeOrientToMovementRotation(CurrentRotation, DeltaTime, DeltaRotation);
	}
	
	return CurrentRotation;
}
