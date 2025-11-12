// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "BTTask_UpdatePatrolPath.h"

#include "AIController.h"
#include "TopDownAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_UpdatePatrolPath::UBTTask_UpdatePatrolPath()
{
	NodeName = "UpdatePatrolPath";
}

EBTNodeResult::Type UBTTask_UpdatePatrolPath::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// Get the blackboard component
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	if (!BlackboardComponent)
	{
		return EBTNodeResult::Failed;
	}

	// Get the AI controller
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// Get the AI Character
	ATopDownAICharacter* AICharacter =  Cast<ATopDownAICharacter>(AIController->GetCharacter());
	if (!AICharacter)
	{
		return EBTNodeResult::Failed;
	}

	// Get the patrol path
    const TArray<TSoftObjectPtr<AActor>>& PatrolPath = AICharacter->GetPatrolPath();
	if (PatrolPath.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	// Get patrol index, increment it, and wrap  to the patrol path array’s valid indices
	int PatrolIndex = BlackboardComponent->GetValueAsInt("PatrolIndex");
	PatrolIndex++;
	PatrolIndex %= PatrolPath.Num();

	// Set new patrol index
	BlackboardComponent->SetValueAsInt("PatrolIndex", PatrolIndex);

	// Set the patrol actor key
	AActor* PatrolActor = PatrolPath[PatrolIndex].Get();
	BlackboardComponent->SetValueAsObject("PatrolActor", PatrolActor);	
	
	return EBTNodeResult::Succeeded;
}
