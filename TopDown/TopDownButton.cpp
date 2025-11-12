// Copyright 2025 Daniel Acevedo - acevedod@usc.edu


#include "TopDownButton.h"
#include "Components/TextBlock.h"

void UTopDownButton::NativeSetSelection(bool bIsSelected)
{
	ButtonTextBlock->SetColorAndOpacity(bIsSelected ? SelectedFontColor : DefaultFontColor);
}

void UTopDownButton::NativeActivated()
{
	// Just trigger the delegate
	OnActivated.Broadcast();
}

void UTopDownButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	
	// Set the text
	ButtonTextBlock->SetText(ButtonText);
	// Set the text color
	ButtonTextBlock->SetColorAndOpacity(DefaultFontColor);
	
}
