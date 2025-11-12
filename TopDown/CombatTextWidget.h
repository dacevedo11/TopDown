// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UCombatTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void ShowDamage(int Damage);

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UTextBlock> CombatTextBlock;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<class UWidgetAnimation> TextFly;

protected:
	// Color of the font when damage is negative (healing)
	UPROPERTY(EditAnywhere)
	FLinearColor HealFontColor = FLinearColor::Green;

	// Color of the font when damage is positive (taking damage)
	UPROPERTY(EditAnywhere)
	FLinearColor DamageFontColor = FLinearColor::Red;
};
