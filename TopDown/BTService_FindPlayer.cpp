// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "BTService_FindPlayer.h"

#include "TopDownAIController.h"
#include "TopDownCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TopDownBPLibrary.h"


UBTService_FindPlayer::UBTService_FindPlayer()
{
	NodeName = "FindPlayer";
 
	PlayerKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindPlayer, PlayerKey), AActor::StaticClass());
	PlayerDistanceKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_FindPlayer, PlayerDistanceKey));
}

void UBTService_FindPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent())
	{
		ATopDownCharacter* PlayerChar = UTopDownBPLibrary::GetTopDownCharacter(&OwnerComp, 0);

		// Make sure player exists and is alive
		if (!PlayerChar || PlayerChar->IsDead()) 
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
			return;
		}

		// Get the AI controller
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (!AIController)
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);		
			return;
		}

		// Get the AI character
		ACharacter* AICharacter = AIController->GetCharacter();
		if (!AICharacter)
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
			return;
		}

		// Get positions
		FVector AILocation = AICharacter->GetActorLocation();
		FVector PlayerLocation = PlayerChar->GetActorLocation();

		// Get threat level
		int ThreatLevel = PlayerChar->GetThreatLevel();

		// Angle and distance parameters
		float MaxAngle = 0.0f;
		float MaxDistance = 0.0f;

		switch (ThreatLevel)
		{
		case 1: // Purple
			MaxAngle = 1.57f;
			MaxDistance = 4000.0f;
			break;
		case 0: // Red
			MaxAngle = 1.22f;
			MaxDistance = 2000.0f;
			break;
		case -1: // Yellow
			MaxAngle = 0.87f;
			MaxDistance = 500.0f;
			break;
		case -2: // Blue
			MaxAngle = 0.70f;
			MaxDistance = 250.0f;
			break;
		default: // Green
			MaxAngle = 0.52f;
			MaxDistance = 100.0f;
			break;
		}

		// Calculate distance
		float Distance = FVector::Dist(PlayerLocation, AILocation);


		// Check if player is too far
		if (Distance > MaxDistance)
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
			return;
		}

		// Calculate angle
		FVector AIForwardVector = AICharacter->GetActorForwardVector();
		FVector ToPlayer = (PlayerLocation - AILocation).GetSafeNormal();
		float DotProduct = FVector::DotProduct(AIForwardVector, ToPlayer);
		float Angle = FMath::Acos(DotProduct);
		
		// Check if player is outside AI vision
		if (FMath::Abs(Angle) > MaxAngle)
		{
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
			return;
		}

		// Line Trace Check
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(AICharacter);

		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			AILocation,
			PlayerLocation,
			ECC_Camera,
			QueryParams
		);

		// Check if we hit the player
		if (bHit && HitResult.GetActor() != PlayerChar)
		{
			// Something is blocking vision
			MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), nullptr);
			return;
		}

		// Player is within distance and angle
		MyBlackboard->SetValueAsObject(GetSelectedPlayerKey(), PlayerChar);
		MyBlackboard->SetValueAsFloat(GetPlayerDistanceKey(), Distance);
		
	}
}

void UBTService_FindPlayer::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		PlayerKey.ResolveSelectedKey(*BBAsset);
		PlayerDistanceKey.ResolveSelectedKey(*BBAsset);
	}
}