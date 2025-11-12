// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "TopDownCharacter.h"
#include "TopDownAICharacter.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API ATopDownAICharacter : public ATopDownCharacter
{
	GENERATED_BODY()

public:
	ATopDownAICharacter();

	const TArray<TSoftObjectPtr<AActor>>& GetPatrolPath() const { return PatrolPath; }

protected:
	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<class UBlackboardData> BlackboardAsset;

	UPROPERTY(EditAnywhere, Category="AI")
	TObjectPtr<class UBehaviorTree> BTAsset;

	friend class ATopDownAIController;

	UPROPERTY(EditInstanceOnly, Category = AI)
	TArray<TSoftObjectPtr<AActor>> PatrolPath;
};
