// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "HUDWidget.h"
#include "TopDownBPLibrary.h"
#include "TopDownCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (ATopDownCharacter* Char = UTopDownBPLibrary::GetTopDownCharacter(this, 0))
	{
		FString HealthStr = FString::FromInt(Char->GetHealth());
		HealthAmount->SetText(FText::FromString(HealthStr));

		float CharHealth = Char->GetHealthPct();
		HealthBar->SetPercent(CharHealth);

		if (CharHealth > 0.7f)
		{
			HealthBar->SetFillColorAndOpacity(GreenColor);
		}
		else if (CharHealth > 0.3f)
		{
			HealthBar->SetFillColorAndOpacity(YellowColor);
		}
		else
		{
			HealthBar->SetFillColorAndOpacity(RedColor);
		}

		// Draw threat level
		int ThreatLevel = Char->GetThreatLevel();

		switch (ThreatLevel)
		{
		case 1:
			StealthTextBlock->SetColorAndOpacity(PurpleColor);
			break;
        
		case 0:
			StealthTextBlock->SetColorAndOpacity(RedColor);
			break;
        
		case -1:
			StealthTextBlock->SetColorAndOpacity(YellowColor);
			break;
        
		case -2:
			StealthTextBlock->SetColorAndOpacity(BlueColor);
			break;
        
		default:
			StealthTextBlock->SetColorAndOpacity(GreenColor);
			break;
		}
	}
}
