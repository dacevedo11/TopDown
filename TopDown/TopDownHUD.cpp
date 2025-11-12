// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "TopDownHUD.h"
#include "DrawDebugHelpers.h"
#include "PuzzleComponent.h"
#include "InteractSubsystem.h"
#include "InteractComponent.h"
#include "TopDownBPLibrary.h"
#include "TopDownCharacter.h"
#include "TopDownGameMode.h"
#include "TopDownPlayerController.h"

static TAutoConsoleVariable<int32> CVarDebugPuzzles(
	TEXT("DebugPuzzles"), // Console command
	0, // Default value
	TEXT("Turns puzzle debug view on/off (off by default)"), // Help text
	ECVF_Default); // Flags


void ATopDownHUD::DrawHUD()
{
	Super::DrawHUD();

	// DrawText(TEXT("Hello World!"), FLinearColor::Red, 0, 0, GEngine->GetSubtitleFont(), 1.0f);
	DrawText(TEXT("Daniel Acevedo - acevedod@usc.edu"), FLinearColor::Black, 10.0, 11.0, GEngine->GetSubtitleFont(), 0.5f);
	DrawText(TEXT("Daniel Acevedo - acevedod@usc.edu"), FLinearColor::Red, 10.0, 10.0, GEngine->GetSubtitleFont(), 0.5f);

	// Interact Hint
	if (UInteractSubsystem* InteractSubsystem = GetWorld()->GetSubsystem<UInteractSubsystem>())
	{
		if (InteractSubsystem->BestCandidate.IsValid())
		{
			UInteractComponent* InteractComponent = InteractSubsystem->BestCandidate.Get();
			if (InteractComponent)
			{
				AActor* OwnerActor = InteractComponent->GetOwner();
				if (OwnerActor)
				{
					FVector ActorLocation = OwnerActor->GetActorLocation();
					FVector2D ScreenPosition;
                    
					if (GetOwningPlayerController()->ProjectWorldLocationToScreen(ActorLocation, ScreenPosition, true))
					{
						const FString HintText = TEXT("[E] - Interact");
						const float TextScale = 1.0f;
						const FLinearColor TextColor = FLinearColor::Red;
                        
						float TextWidth, TextHeight;
						GetTextSize(HintText, TextWidth, TextHeight, nullptr, TextScale);
                        
						float CenteredX = ScreenPosition.X - (TextWidth * 0.5f);
						float CenteredY = ScreenPosition.Y - (TextHeight * 0.5f);
                        
						DrawText(HintText, TextColor, CenteredX, CenteredY, nullptr, TextScale);
					}
				}
			}
		}

		if (ATopDownCharacter* Char = UTopDownBPLibrary::GetTopDownCharacter(this, 0))
		{
			DrawText(Char->GetName(), FLinearColor::Green, 20, 75, nullptr);
		}

		if (ATopDownPlayerController* Controller = UTopDownBPLibrary::GetTopDownController(this, 0))
		{
			DrawText(Controller->GetName(), FLinearColor::Green, 20, 100, nullptr);
		}

		if (ATopDownGameMode* GameMode = UTopDownBPLibrary::GetTopDownGameMode(this))
		{
			DrawText(GameMode->GetName(), FLinearColor::Green, 20, 125, nullptr);
		}
	

#if ENABLE_DRAW_DEBUG
		if (CVarDebugPuzzles.GetValueOnGameThread() != 0)
		{
			for (TObjectIterator<UPuzzleComponent> Itr; Itr; ++Itr)
			{
				Itr->DebugDrawOutput();
			}		
		}
#endif
	}
}
