// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TopDownBPLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UTopDownBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	static class ATopDownCharacter* GetTopDownCharacter(const UObject* WorldContextObject, int32 PlayerIndex = 0);
    
	UFUNCTION(BlueprintPure)
	static class ATopDownPlayerController* GetTopDownController(const UObject* WorldContextObject, int32 PlayerIndex = 0);
    
	UFUNCTION(BlueprintPure)
	static class ATopDownGameMode* GetTopDownGameMode(const UObject* WorldContextObject);
};
