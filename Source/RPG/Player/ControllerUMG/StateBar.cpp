// Fill out your copyright notice in the Description page of Project Settings.


#include "StateBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool UStateBar::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	StateProgressBar = Cast<UProgressBar>(GetWidgetFromName("StateBar"));
	StateText = Cast<UTextBlock>(GetWidgetFromName("StateText"));
	
	return true;
}

void UStateBar::UpdateState(float CurrentValue, float MaxValue)
{
	StateProgressBar->SetPercent(CurrentValue/MaxValue);
	const FString Current = FString::FromInt(CurrentValue);
	const FString Max = FString::FromInt(MaxValue);
	const FString Value = Current + "/" + Max;
	StateText->SetText(FText::FromString(Value));
}
