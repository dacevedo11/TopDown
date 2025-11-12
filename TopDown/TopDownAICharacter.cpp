// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "TopDownAICharacter.h"
#include "TopDownAIController.h"

ATopDownAICharacter::ATopDownAICharacter()
{
	AIControllerClass = ATopDownAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned; 
}
