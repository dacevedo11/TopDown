// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "TopDownAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "TopDownCharacter.h"


UTopDownAttributeSet::UTopDownAttributeSet()
{
	InitHealth(150.0f);
	InitMaxHealth(150.0f);
	InitMoveSpeed(600.0f);
}

void UTopDownAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	
	Super::PreAttributeChange(Attribute, NewValue);
}

void UTopDownAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		// We're applying damage, first calculate how much
		float DamageValue = GetDamage();

		// If the target is invulnerable, don't apply any damage
		const FGameplayTag InvulnTag = FGameplayTag::RequestGameplayTag("Buff.Invulnerable", false);
		if (Data.Target.HasMatchingGameplayTag(InvulnTag))
		{
			DamageValue = 0.0f;
		}

		// Calculate what the new health should be
		const float NewHealth = FMath::Clamp(GetHealth() - DamageValue, 0.0f, GetMaxHealth());

		if (NewHealth != GetHealth())
		{
			SetHealth(NewHealth);
			
			if (ATopDownCharacter* TopDownChar = Cast<ATopDownCharacter>(Data.Target.GetOwnerActor()))
			{
				TopDownChar->HandleDamage(DamageValue, NewHealth);
			}
			
		}

		// We've applied this damage, so clear it out
		SetDamage(0.0f);
	}
}
