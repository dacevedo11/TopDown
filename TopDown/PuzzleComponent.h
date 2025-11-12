// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PuzzleComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FPuzzleComponentOnReceiveInput, class AActor*, InputActor, bool, bValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPuzzleComponentOnPuzzleReset);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWN_API UPuzzleComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPuzzleComponent();

	UPROPERTY(BlueprintAssignable)
	FPuzzleComponentOnReceiveInput OnReceiveInput;

	UPROPERTY(BlueprintAssignable)
	FPuzzleComponentOnPuzzleReset OnPuzzleReset;

	/**
	* Sets whether this puzzle's component is active
	* @param bNewOutputActive - whether the output should be active (true) or inactive (false)
	* @return void
	*/
	UFUNCTION(BlueprintCallable)
	virtual void SetOutputActive(bool bNewOutputActive);

	// Receives input from another puzzle component
	virtual void NativeReceiveInput(TObjectPtr<AActor> Sender, bool bIncomingValue);

	// PuzzleReset - (UFUNCTION that’s BlueprintCallable) a virtual function that returns void and takes no parameters
	UFUNCTION(BlueprintCallable)
	virtual void PuzzleReset();

	// 
	void DebugDrawOutput();

	UFUNCTION(BlueprintCallable)
	void ChangeOutputActor(AActor* NewOutputActor);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// A bool UPROPERTY that’s Transient and BlueprintReadOnly, defaults to false. This will be used to track whether or out the output is active
	UPROPERTY(Transient, BlueprintReadOnly)
	bool bIsOutputActive = false;

	// A TSoftObjectPtr<AActor> UPROPERTY that’s EditInstanceOnly. This will be used to track which actor this puzzle component should send the output to
	UPROPERTY(EditInstanceOnly)
	TSoftObjectPtr<AActor> OutputActor;
};
