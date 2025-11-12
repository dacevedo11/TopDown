// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_TryActivateAbility.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UBTTask_TryActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_TryActivateAbility();

	UPROPERTY(EditAnywhere)
	FName AbilityName;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;

	void OnAbilityEnded(const struct FAbilityEndedData& Data, TWeakObjectPtr<class UTopDownAbilitySystemComponent> ASC);

private:
	// Used to track the ability delegate we add
	FDelegateHandle OnAbilityEndedHandle;

	// Helper to track the BT component
	UPROPERTY()
	UBehaviorTreeComponent* MyOwnerComp;

	
};
