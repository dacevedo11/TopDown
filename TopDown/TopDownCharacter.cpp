// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownCharacter.h"

#include "CombatTextWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "TopDownAbilitySystemComponent.h"
#include "TopDownAttributeSet.h"
#include "TopDownCharMoveComponent.h"
#include "TopDownPlayerController.h"
#include "Components/Widget.h"
#include "Components/WidgetComponent.h"

ATopDownCharacter::ATopDownCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UTopDownCharMoveComponent>(CharacterMovementComponentName))
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to the camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create the camera boom component
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));

	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	// Create the camera component
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));

	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	// Activate ticking to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	AbilitySystemComponent = CreateDefaultSubobject<UTopDownAbilitySystemComponent>(TEXT("AbilitySystem"));

	DefaultAttributeSet = CreateDefaultSubobject<UTopDownAttributeSet>(TEXT("DefaultAttributeSet"));

	CombatTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("CombatTextWidget"));

	// Create the CombatTextWidget component
	CombatTextWidget->SetupAttachment(RootComponent);
	CombatTextWidget->SetWidgetSpace(EWidgetSpace::Screen);

	// Initialize Gameplay Tags
	StealthTag = FGameplayTag::RequestGameplayTag("Buff.Stealth");
	CrouchTag = FGameplayTag::RequestGameplayTag("Ability.Crouch");
	DeathTag = FGameplayTag::RequestGameplayTag("Debuff.Immobile.Dead");
}

void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
	}

	// Grant initial abilities
	for (const auto& Ability : InitialGameplayAbilities)
	{
		GiveAbility(Ability.Key, Ability.Value);
	}

	// Apply initial effects
	for (const auto& Effect : InitialGameplayEffects)
	{
		if (Effect)
		{
			FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1.0f, ContextHandle);
			if (SpecHandle.IsValid())
			{
				AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}
}

void ATopDownCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

	// stub
}

void ATopDownCharacter::OnEnterBlueTrigger()
{
	NumBlueTriggers++;
	if (NumBlueTriggers == 1)
	{
		FVector NewColor(BlueColor.R, BlueColor.G, BlueColor.B);
		GetMesh()->SetVectorParameterValueOnMaterials(BodyColorParameter, NewColor);	
	}
}

void ATopDownCharacter::OnExitBlueTrigger()
{
	NumBlueTriggers--;
	if (NumBlueTriggers == 0)
	{
		FVector NewColor(DefaultColor.R, DefaultColor.G, DefaultColor.B);
		GetMesh()->SetVectorParameterValueOnMaterials(BodyColorParameter, NewColor);
	}
}

UAbilitySystemComponent* ATopDownCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATopDownCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	// Apply the crouching effect to myself
	FGameplayEffectContextHandle ContextHandle;
	CrouchingEffectHandle = AbilitySystemComponent->BP_ApplyGameplayEffectToSelf(CrouchingEffect, 0.0f, ContextHandle);
}

void ATopDownCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	Super::OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);

	// Remove the crouching effect
	AbilitySystemComponent->RemoveActiveGameplayEffect(CrouchingEffectHandle);
	CrouchingEffectHandle.Invalidate();
}

void ATopDownCharacter::GiveAbility(const FName& AbilityName, TSubclassOf<class UGameplayAbility> AbilityClass)
{
	AbilitySystemComponent->GiveAbility(AbilityClass);
	GameplayAbilities.Emplace(AbilityName, AbilityClass);
}

bool ATopDownCharacter::TryActivateAbility(const FName& AbilityName)
{
	if (GameplayAbilities.Contains(AbilityName))
	{
		return AbilitySystemComponent->TryActivateAbilityByClass(GameplayAbilities[AbilityName]);
	}

	return false;
}

void ATopDownCharacter::HandleDamage(float Amount, float NewHealth)
{
	// Show damage text
	if (CombatTextWidget)
	{
		if (UUserWidget* Widget = CombatTextWidget->GetWidget())
		{
			if (UCombatTextWidget* CombatText = Cast<UCombatTextWidget>(Widget))
			{
				CombatText->ShowDamage(static_cast<int>(Amount));
			}
		}
	}
	
	// Death
	if (NewHealth == 0.0f)
	{
		TryActivateAbility(FName("Death"));

		// Call OnDeath in TopDownPlayerController
		if (ATopDownPlayerController* TopDownPlayerController = Cast<ATopDownPlayerController>(GetController()))
		{
			TopDownPlayerController->OnDeath();
		}
	}
}

void ATopDownCharacter::EnableRagdoll()
{
	// Turn on physics on the SMC so it goes into ragdoll
	USkeletalMeshComponent* SMC = GetMesh();
	SMC->SetSimulatePhysics(true);
	SMC->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SMC->SetAllBodiesSimulatePhysics(true);
	SMC->SetAllBodiesPhysicsBlendWeight(1.0f);

	// Disable collision on the capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

float ATopDownCharacter::GetHealth() const
{
	return DefaultAttributeSet ? DefaultAttributeSet->GetHealth() : 0.f;
}

float ATopDownCharacter::GetMaxHealth() const
{
	return DefaultAttributeSet ? DefaultAttributeSet->GetMaxHealth() : 1.f;
}

float ATopDownCharacter::GetHealthPct() const
{
	const float MaxHealth = GetMaxHealth();
	return MaxHealth > 0.f ? GetHealth() / MaxHealth : 1.f;
}

int ATopDownCharacter::GetThreatLevel() const
{
	int ThreatLevel = 0;

	if (AbilitySystemComponent)
	{
		// If the character is currently moving, the threat level should increase by one tier
		UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
		if (CharacterMovementComponent)
		{
			FVector Velocity = CharacterMovementComponent->Velocity;
			if (Velocity.Size2D() > 0.0f)
			{
				ThreatLevel++;
			}
		}

		// If the character is currently crouching, the threat level should decrease by one tier
		ThreatLevel -= AbilitySystemComponent->GetGameplayTagCount(CrouchTag);

		// If the character is currently inside stealth triggers, the threat level should decrease by one tier per stealth trigger
		ThreatLevel -= AbilitySystemComponent->GetGameplayTagCount(StealthTag);
	}

	return ThreatLevel;
}

bool ATopDownCharacter::IsDead() const
{
	if (AbilitySystemComponent)
	{
		return AbilitySystemComponent->HasMatchingGameplayTag(DeathTag);
	}

	return false;
}
