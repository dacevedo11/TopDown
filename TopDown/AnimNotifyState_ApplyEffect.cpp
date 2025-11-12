// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "AnimNotifyState_ApplyEffect.h"

#include "AbilitySystemComponent.h"
#include "TopDownCharacter.h"

void UAnimNotifyState_ApplyEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	ATopDownCharacter* TopDownChar = MeshComp->GetOwner<ATopDownCharacter>();
	if (TopDownChar == nullptr)
	{
		return;
	}

	// Apply the effect
	if (EffectToApply)
	{
		FGameplayEffectContextHandle ContextHandle;
		EffectHandle = TopDownChar->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToSelf(EffectToApply, 0.0f, ContextHandle);
	}
}

void UAnimNotifyState_ApplyEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ATopDownCharacter* TopDownChar = MeshComp->GetOwner<ATopDownCharacter>();
	if (TopDownChar == nullptr)
	{
		return;
	}

	// Remove the effect
	TopDownChar->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(EffectHandle);
	EffectHandle.Invalidate();
}
