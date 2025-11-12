// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "BTTask_TryActivateAbility.h"
#include "AbilitySystemComponent.h"
#include "AIController.h"
#include "TopDownAbilitySystemComponent.h"
#include "TopDownCharacter.h"

UBTTask_TryActivateAbility::UBTTask_TryActivateAbility()
{
	NodeName = "Try Activate Ability";

	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTTask_TryActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	MyOwnerComp = &OwnerComp;

	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Aborted;
	}

	// Get the TopDown character
	ATopDownCharacter* Char = Cast<ATopDownCharacter>(AIController->GetCharacter());
	if (!Char)
	{
		return EBTNodeResult::Aborted;
	}

	// Try to activate ability
	if (Char->TryActivateAbility(AbilityName))
	{
        UTopDownAbilitySystemComponent* ASC = Char->AbilitySystemComponent;
		OnAbilityEndedHandle = ASC->OnAbilityEnded.AddUObject(this, &UBTTask_TryActivateAbility::OnAbilityEnded, TWeakObjectPtr<UTopDownAbilitySystemComponent>(ASC));

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Aborted	;
}

FString UBTTask_TryActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s: '%s'"), *Super::GetStaticDescription(), *AbilityName.ToString());
}

void UBTTask_TryActivateAbility::OnAbilityEnded(const struct FAbilityEndedData& Data, TWeakObjectPtr<class UTopDownAbilitySystemComponent> ASC)
{
	// If the handle and the ASC are both valid, remove the delegate binding
	if (OnAbilityEndedHandle.IsValid() && ASC.IsValid())
	{
		ASC.Get()->OnAbilityEnded.Remove(OnAbilityEndedHandle);
	}

	// This lets the behavior tree know this task is done
	FinishLatentTask(*MyOwnerComp, EBTNodeResult::Succeeded);

}
