// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "TopDownBPLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownCharacter.h"
#include "TopDownPlayerController.h"
#include "TopDownGameMode.h"

ATopDownCharacter* UTopDownBPLibrary::GetTopDownCharacter(const UObject* WorldContextObject, int32 PlayerIndex)
{
	return Cast<ATopDownCharacter>(UGameplayStatics::GetPlayerCharacter(WorldContextObject, PlayerIndex));
}

ATopDownPlayerController* UTopDownBPLibrary::GetTopDownController(const UObject* WorldContextObject, int32 PlayerIndex)
{
	return Cast<ATopDownPlayerController>(UGameplayStatics::GetPlayerController(WorldContextObject, PlayerIndex));
}

ATopDownGameMode* UTopDownBPLibrary::GetTopDownGameMode(const UObject* WorldContextObject)
{
	return Cast<ATopDownGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
}