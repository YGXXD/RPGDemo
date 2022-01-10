// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberText.h"

#include "Components/TextBlock.h"
#include "SlateCore/Public/Styling/SlateColor.h"

bool UNumberText::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	// 获取UI动画
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	// 播放动画
	if(WidgetClass->Animations.Num())
	{
		PlayAnimation(WidgetClass->Animations[0]);
	}
	
	NumberTextBlock = Cast<UTextBlock>(GetWidgetFromName("NumberTextBlock"));
	
	return true;
}

void UNumberText::SetNumberTextValue(EAttributeType Key, float ModifyValue)
{
	if(NumberTextBlock)
	{
		const FString Plus = "+";
		FString Value = "";
		if(ModifyValue > 0)
		{
			Value = Plus + FString::FromInt(ModifyValue);
		}
		else if(ModifyValue < 0)
		{
			Value = FString::FromInt(ModifyValue);
		}
		switch (Key)
		{
		case EAttributeType::Health:
			NumberTextBlock->SetText(FText::FromString(Value));
			if(ModifyValue >= 0)
			{
				NumberTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0,1,0)));
			}
			else if(ModifyValue < 0)
			{
				NumberTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1,0,0)));
			}
			break;
			
		case EAttributeType::Mana:
			NumberTextBlock->SetText(FText::FromString(Value));
			NumberTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0,0,1)));
			break;
		
		case EAttributeType::Stamina:
			NumberTextBlock->SetText(FText::FromString(Value));
			NumberTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0.3,1,0.3)));
			break;

		case EAttributeType::Experience:
			if(ModifyValue > 0)
			{
				NumberTextBlock->Font.Size = 20;
				NumberTextBlock->SetText(FText::FromString(Value));
				NumberTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1,1,0)));
			}
			break;

		case EAttributeType::Level:
			NumberTextBlock->SetText(FText::FromName(TEXT("Level Up")));
			NumberTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(1,1,1)));
			break;
			
		default:
			break;
		}
		
	}
}
