// Fill out your copyright notice in the Description page of Project Settings.


#include "TalkBoard.h"

#include "Components/TextBlock.h"

bool UTalkBoard::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	SetVisibility(ESlateVisibility::Collapsed);
	NameText = Cast<UTextBlock>(GetWidgetFromName("NameText"));
	TalkInfoText = Cast<UTextBlock>(GetWidgetFromName("TalkInfoText"));

	return true;
}

void UTalkBoard::ShowTalkInfo(FString Name, FString TalkInfo)
{
	if(NameText)
	{
		NameText->SetText(FText::FromString(Name+":"));
	}
	if(TalkInfoText)
	{
		TalkInfoText->SetText(FText::FromString(TalkInfo));
	}
	SetVisibility(ESlateVisibility::Visible);
}

void UTalkBoard::HideTalkInfo()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

