// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "CombatTextWidget.h"
#include "Components/TextBlock.h"

void UCombatTextWidget::ShowDamage(int Damage)
{
	if (Damage > 0)
	{
		CombatTextBlock->SetColorAndOpacity(DamageFontColor);
	}
	else 
	{
		CombatTextBlock->SetColorAndOpacity(HealFontColor);
	}

    CombatTextBlock->SetText(FText::AsNumber(FMath::Abs(Damage)));

	PlayAnimation(TextFly);
}
