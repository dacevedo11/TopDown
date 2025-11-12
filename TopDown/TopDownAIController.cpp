// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#include "TopDownAIController.h"
#include "TopDownAICharacter.h"
#include "TopDownCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

void ATopDownAIController::OnPossess(APawn* InPawn)
{
	OwnerCharacter = Cast<ATopDownAICharacter>(InPawn);
	
	bool bInitializedBB = false; 
	if (OwnerCharacter && OwnerCharacter->BlackboardAsset && OwnerCharacter->BTAsset)
	{
		UBlackboardComponent* BBComponent = nullptr;
		if (UseBlackboard(OwnerCharacter->BlackboardAsset, BBComponent))
		{
			BBComponent->SetValueAsInt("PatrolIndex", -1);
			RunBehaviorTree(OwnerCharacter->BTAsset);
			bInitializedBB = true;
		}
	}

	if (!bInitializedBB)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATopDownAIController can't initialize Behavior Tree for %s"), *InPawn->GetName());
	}
	
	Super::OnPossess(InPawn);
}
