// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "FireComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFireComponentOnCatchFire);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWN_API UFireComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	UFireComponent();

	UFUNCTION()
	void HandleOnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UPROPERTY(BlueprintAssignable)
	FFireComponentOnCatchFire OnCatchFire;

	virtual void NativeCatchFire();

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bStartsOnFire;
	
	UPROPERTY(BlueprintReadOnly)
	bool bIsOnFire;

	UPROPERTY(EditAnywhere)
	TObjectPtr <USoundBase> FireSoundBase;

	UPROPERTY(EditAnywhere)
	TObjectPtr <UParticleSystem> FireParticleSystem;
};
