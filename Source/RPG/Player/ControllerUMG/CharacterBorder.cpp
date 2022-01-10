// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterBorder.h"

#include "StateBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool UCharacterBorder::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	Money = 0;
	TargetMoney = 0;
	
	HealthBar = Cast<UStateBar>(GetWidgetFromName("HealthBar"));
	ManaBar = Cast<UStateBar>(GetWidgetFromName("ManaBar"));
	StaminaBar = Cast<UProgressBar>(GetWidgetFromName("StaminaBar"));
	LevelText = Cast<UTextBlock>(GetWidgetFromName("LevelNumText"));
	MoneyNumText = Cast<UTextBlock>(GetWidgetFromName("MoneyNumText"));

	return true;
}

void UCharacterBorder::UpdateCharacterBorder(EAttributeType Key, float CurrentValue, float MaxValue)
{
	switch (Key)
	{
	case EAttributeType::Health:
		UpdateHealthBar(CurrentValue,MaxValue);
		break;
		
	case EAttributeType::Mana:
		UpdateManaBar(CurrentValue,MaxValue);
		break;
		
	case EAttributeType::Stamina:
		UpdateStaminaBar(CurrentValue,MaxValue);
		break;
		
	case EAttributeType::Level:
		UpdateLevelText(CurrentValue);
		break;

	case EAttributeType::Money:
		UpdateMoneyNumText(CurrentValue);
		break;
		
	default:
		break;
	}
}

void UCharacterBorder::UpdateHealthBar(float CurrentValue, float MaxValue)
{
	HealthBar->UpdateState(CurrentValue,MaxValue);
}

void UCharacterBorder::UpdateManaBar(float CurrentValue, float MaxValue)
{
	ManaBar->UpdateState(CurrentValue,MaxValue);
}

void UCharacterBorder::UpdateStaminaBar(float CurrentValue, float MaxValue)
{
	StaminaBar->SetPercent(CurrentValue/MaxValue);
}

void UCharacterBorder::UpdateLevelText(float CurrentValue)
{
	if(CurrentValue < 10)
	{
		const FString Value = "0"+FString::FromInt(CurrentValue);
		LevelText->SetText(FText::FromString(Value));
	}
	else
	{
		LevelText->SetText(FText::AsNumber(static_cast<INT>(CurrentValue)));
	}
}

void UCharacterBorder::UpdateMoneyNumText(float CurrentValue)
{
	TargetMoney = CurrentValue;
	auto MoneyUpdateLambda = [this]()
	{
		if(TargetMoney > Money)
		{
			Money++;
		}
		else if(TargetMoney < Money)
		{
			Money--;
		}
		if(MoneyNumText)
		{
			MoneyNumText->SetText(FText::AsNumber(static_cast<INT>(Money)));
		}
		if(Money == TargetMoney)
		{
			GetWorld()->GetTimerManager().PauseTimer(MoneyUpdateHandle);
		}
	};
	const FTimerDelegate MoneyUpdateDelegate = FTimerDelegate::CreateLambda(MoneyUpdateLambda);
	GetWorld()->GetTimerManager().SetTimer(MoneyUpdateHandle,MoneyUpdateDelegate,0.002,true);
	
}


