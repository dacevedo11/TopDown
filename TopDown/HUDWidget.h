// Copyright 2025 Daniel Acevedo - acevedod@usc.edu

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWN_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> HealthAmount;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> HealthBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> StealthTextBlock;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor GreenColor = FLinearColor::Green;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor YellowColor = FLinearColor::Yellow;
	
	UPROPERTY(EditDefaultsOnly)
	FLinearColor RedColor = FLinearColor::Red;

	UPROPERTY(EditDefaultsOnly)
	FLinearColor PurpleColor = FLinearColor(0.5f, 0.0f, 1.0f, 1.0f);

	UPROPERTY(EditDefaultsOnly)
	FLinearColor BlueColor = FLinearColor::Blue;
};
