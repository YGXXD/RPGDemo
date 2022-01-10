// Fill out your copyright notice in the Description page of Project Settings.


#include "NumberWidgetComponent.h"

#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Player/ControllerUMG/NumberText.h"


UNumberWidgetComponent::UNumberWidgetComponent()
{
	SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UNumberText> WBP_NumberText(TEXT("/Game/RPG_Game/UI/WBP_NumberText"));
	if(WBP_NumberText.Succeeded())
	{
		WidgetClass = WBP_NumberText.Class;
	}
}

void UNumberWidgetComponent::AddNumberTextToView(EAttributeType Key,float Value)
{
	UNumberText* NumberText = Cast<UNumberText>(GetUserWidgetObject());
	if(NumberText)
	{
		NumberText->SetNumberTextValue(Key,Value);
	}
	
	const FLatentActionInfo DelayInfo(1,1,TEXT("DelayDestroy"),this);
	UKismetSystemLibrary::Delay(this, 1.0f, DelayInfo);
}

void UNumberWidgetComponent::DelayDestroy()
{
	DestroyComponent();
}
