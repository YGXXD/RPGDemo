// Fill out your copyright notice in the Description page of Project Settings.


#include "NeutralObjectInteraction.h"

#include "Components/TextBlock.h"

bool UNeutralObjectInteraction::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	bIsInteracting = false;
	
	NameText = Cast<UTextBlock>(GetWidgetFromName("NameText"));
	InteractionText = Cast<UTextBlock>(GetWidgetFromName("InteractionText"));
	TalkInfoText = Cast<UTextBlock>(GetWidgetFromName("TalkInfoText"));
	if(TalkInfoText)
	{
		TalkInfoText->SetVisibility(ESlateVisibility::Collapsed);
	}

	return true;
}

void UNeutralObjectInteraction::SetNameText(FString Name)
{
	if(NameText)
	{
		NameText->SetText(FText::FromString(Name));
	}
}

void UNeutralObjectInteraction::ShowTalkInfo(FText TalkInfo)
{
	if(bIsInteracting)
	{
		return;
	}
	bIsInteracting = true;
	if(NameText)
	{
		NameText->SetVisibility(ESlateVisibility::Collapsed);
	}
	if(InteractionText)
	{
		InteractionText->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	if(TalkInfoText)
	{
		TalkInfoText->SetText(TalkInfo);
		TalkInfoText->SetVisibility(ESlateVisibility::Visible);
	}
	auto HideTalkInfoTextLambda = [this]()
	{
		bIsInteracting = false;
		if(NameText)
		{
			NameText->SetVisibility(ESlateVisibility::Visible);
		}
		if(InteractionText)
		{
			InteractionText->SetVisibility(ESlateVisibility::Visible);
		}
		if(TalkInfoText)
		{
			TalkInfoText->SetVisibility(ESlateVisibility::Collapsed);
		}
	};
	const FTimerDelegate HideTalkInfoTextDelegate = FTimerDelegate::CreateLambda(HideTalkInfoTextLambda);
	GetWorld()->GetTimerManager().SetTimer(HideTalkInfoTextHandle,HideTalkInfoTextDelegate,5,false);
}
