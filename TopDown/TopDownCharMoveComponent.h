// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TopDownAttributeSet.h"
#include "TopDownCharMoveComponent.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UTopDownCharMoveComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

	virtual FRotator ComputeOrientToMovementRotation(const FRotator& CurrentRotation, float DeltaTime, FRotator& DeltaRotation) const override;

private:
	TObjectPtr<class ATopDownCharacter> TopDownChar;
};
