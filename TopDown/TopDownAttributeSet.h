// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "TopDownAbilitySystemComponent.h"
#include "TopDownAttributeSet.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UTopDownAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UTopDownAttributeSet();

	// Current health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Health;
   
	// Maximum health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MaxHealth;
   
	// Movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData MoveSpeed;

	// Damage
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayAttributeData Damage;
   
	// Define getters/setters
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownAttributeSet, Health);
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownAttributeSet, MoveSpeed);
	ATTRIBUTE_ACCESSORS_BASIC(UTopDownAttributeSet, Damage);

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
};
