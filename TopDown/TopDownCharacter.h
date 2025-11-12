// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "TopDownCharacter.generated.h"

/**
 *  A controllable top-down perspective character
 */
UCLASS(abstract)
class ATopDownCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor DefaultColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor BlueColor = FLinearColor::Blue;

	UPROPERTY(EditDefaultsOnly)
	FName BodyColorParameter = FName(TEXT("Paint Tint"));

	bool bIsDead = false;

protected:
	UPROPERTY(BlueprintReadOnly)
	int NumBlueTriggers = 0;

	// The actual crouching effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TSubclassOf<class UGameplayEffect> CrouchingEffect;

	// The handle to track when the crouching effect is active
	FActiveGameplayEffectHandle CrouchingEffectHandle;

	// Current Gameplay Abilities
	// Do not directly change this and instead use "GiveAbility"
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Abilities)
	TMap<FName, TSubclassOf<class UGameplayAbility>> GameplayAbilities;

	FGameplayTag StealthTag;
	FGameplayTag CrouchTag;
	FGameplayTag DeathTag;

public:
	/** Constructor */
	ATopDownCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/** Initialization */
	virtual void BeginPlay() override;

	/** Update */
	virtual void Tick(float DeltaSeconds) override;

	/** Returns the camera component **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }

	/** Returns the Camera Boom component **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UFUNCTION(BlueprintCallable)
	void OnEnterBlueTrigger();

	UFUNCTION(BlueprintCallable)
	void OnExitBlueTrigger();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Abilities)
	TObjectPtr<class UTopDownAbilitySystemComponent> AbilitySystemComponent;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY()
	TObjectPtr<class UTopDownAttributeSet> DefaultAttributeSet;

	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	// Gameplay abilities that should be given on begin play
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TMap<FName, TSubclassOf<class UGameplayAbility>> InitialGameplayAbilities;

	/**
	 * Give the character a gameplay ability and track it with a specific name
	 *
	 * @param AbilityName - Name of the ability
	 * @param AbilityClass - Which ability class to grant
	 */
	UFUNCTION(BlueprintCallable, Category = Abilities)
	void GiveAbility(const FName& AbilityName, TSubclassOf<class UGameplayAbility> AbilityClass);

	/**
	* Try to activate the ability by the given name
	* 
	* @param AbilityName - Name of the ability to activate
	* @return true if it thinks the ability activation worked
	*/
	UFUNCTION(BlueprintCallable, Category = Abilities)
	bool TryActivateAbility(const FName& AbilityName);

	// Called from the AttributeSet when damage is taken
	void HandleDamage(float Amount, float NewHealth);

	// Called to turn on ragdoll on the character
	UFUNCTION(BlueprintCallable, Category = Abilities)
	void EnableRagdoll();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable)
	float GetHealthPct() const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UWidgetComponent> CombatTextWidget;

	int GetThreatLevel() const;

	bool IsDead() const;

	// Gameplay effects that should be applied on begin play
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities)
	TArray<TSubclassOf<class UGameplayEffect>> InitialGameplayEffects;
};

