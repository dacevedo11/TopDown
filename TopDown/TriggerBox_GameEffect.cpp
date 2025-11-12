// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "TriggerBox_GameEffect.h"
#include "TopDownCharacter.h"
#include "Components/ShapeComponent.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"

void ATriggerBox_GameEffect::BeginPlay()
{
	Super::BeginPlay();

	GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATriggerBox_GameEffect::HandleOnBeginOverlap);
	GetCollisionComponent()->OnComponentEndOverlap.AddDynamic(this, &ATriggerBox_GameEffect::HandleOnEndOverlap);
}

void ATriggerBox_GameEffect::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetCollisionComponent()->OnComponentBeginOverlap.RemoveAll(this);
	GetCollisionComponent()->OnComponentEndOverlap.RemoveAll(this);
	
}

void ATriggerBox_GameEffect::HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ATopDownCharacter* TopDownChar = Cast<ATopDownCharacter>(OtherActor))
	{
		if (GameplayEffect)
		{
			FGameplayEffectContextHandle ContextHandle;
			FActiveGameplayEffectHandle EffectHandle = TopDownChar->GetAbilitySystemComponent()->BP_ApplyGameplayEffectToSelf(GameplayEffect, 0.0f, ContextHandle);
			GameplayEffectHandles.Emplace(TopDownChar, EffectHandle);
		}
	}
}

void ATriggerBox_GameEffect::HandleOnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ATopDownCharacter* TopDownChar = Cast<ATopDownCharacter>(OtherActor))
	{
		if (GameplayEffectHandles.Contains(TopDownChar))
		{
			FActiveGameplayEffectHandle EffectHandle = GameplayEffectHandles[TopDownChar];
			TopDownChar->GetAbilitySystemComponent()->RemoveActiveGameplayEffect(EffectHandle);
			EffectHandle.Invalidate();
			GameplayEffectHandles.Remove(TopDownChar);
		}
	}
}
