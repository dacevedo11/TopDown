// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "TopDownCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "InteractSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "TopDown.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "TopDownCharacter.h"
#include "HUDWidget.h" 
#include "TopDownMenu.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void ATopDownPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (HUDWidgetClass)
	{
		HUDWidgetInstance = NewObject<UHUDWidget>(this, HUDWidgetClass);
		HUDWidgetInstance->AddToViewport();
	}
}

void ATopDownPlayerController::OnDeath()
{
	if (ATopDownCharacter* Char = GetPawn<ATopDownCharacter>())
	{
			UAbilitySystemComponent* AbilitySystemComponent = Char->GetAbilitySystemComponent();
			if (AbilitySystemComponent != nullptr)
			{
				MenuInstance = NewObject<UTopDownMenu>(this, MenuWidgetClass);
				MenuInstance->AddToViewport();
				
				// For a widget which we want to accept input from a player,
				// we have to set the owning player controller
				MenuInstance->SetOwningPlayer(this);
				// InputMode UI means the input is only routed to the UI,
				// and we specify which widget should receive input
				UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, MenuInstance);
			}
	}
}

ATopDownPlayerController::ATopDownPlayerController()
{
	bShowMouseCursor = false;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ATopDownPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Only set up input on local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		// Set up action bindings
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnMoveForward);
			EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnMoveRight);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnJumpAction);
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnInteractAction);
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnCrouchAction);
			EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ATopDownPlayerController::OnDashAction);
		}
		else
		{
			UE_LOG(LogTopDown, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
		}
	}
}

void ATopDownPlayerController::OnJumpAction()
{
	if (ACharacter* MyCharacter = GetCharacter())
	{
		MyCharacter->Jump();
	}
}

void ATopDownPlayerController::OnMoveForward(const FInputActionInstance& Instance)
{
	if (APawn* MyPawn = GetPawn())
	{
		float AxisValue = Instance.GetValue().Get<float>();
		MyPawn->AddMovementInput(FVector::ForwardVector, AxisValue);
	}
}

void ATopDownPlayerController::OnMoveRight(const FInputActionInstance& Instance)
{
	if (APawn* MyPawn = GetPawn())
	{
		float AxisValue = Instance.GetValue().Get<float>();
		MyPawn->AddMovementInput(FVector::RightVector, AxisValue);
	}
}

void ATopDownPlayerController::OnInteractAction()
{
	if (UInteractSubsystem* Subsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		Subsystem->PerformInteract();
	}
}

void ATopDownPlayerController::OnCrouchAction()
{
	ATopDownCharacter* TopDownChar = Cast<ATopDownCharacter>(GetCharacter());
	if (TopDownChar == nullptr)
	{
		return;
	}

	const FGameplayTag ImmobileTag = FGameplayTag::RequestGameplayTag("Debuff.Immobile", false);
	if (TopDownChar->GetAbilitySystemComponent()->HasMatchingGameplayTag(ImmobileTag))
	{
		return;	
	}

	if (ACharacter* MyCharacter = GetCharacter())
	{
		if (MyCharacter->IsCrouched())
		{
			MyCharacter->UnCrouch();
		}
		else
		{
			MyCharacter->Crouch();
		}
	}
}

void ATopDownPlayerController::OnDashAction()
{
	ATopDownCharacter* TopDownChar = Cast<ATopDownCharacter>(GetCharacter());
	if (TopDownChar == nullptr)
	{
		return;
	}
	
	TopDownChar->TryActivateAbility("Dash");
}
