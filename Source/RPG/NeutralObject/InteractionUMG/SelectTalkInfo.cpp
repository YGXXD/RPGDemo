// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectTalkInfo.h"

#include "SelectTalkBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool USelectTalkInfo::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	TalkTypeImage = Cast<UImage>(GetWidgetFromName("TalkTypeImage"));
	SelectTalkInfoText = Cast<UTextBlock>(GetWidgetFromName("SelectTalkInfoText"));
	SelectTalkButton = Cast<UButton>(GetWidgetFromName("SelectTalkButton"));
	if(SelectTalkButton)
	{
		SelectTalkButton->OnClicked.AddDynamic(this,&USelectTalkInfo::OnSelectTalkButtonClick);
	}
	
	return true;
}

void USelectTalkInfo::OnSelectTalkButtonClick()
{
	if(SelectTalkBox)
	{
		switch (SubTalkType)
		{
		case ESubTalkType::None:
			SelectTalkBox->SelectTalkInfo(TalkIndex);
			break;

		case ESubTalkType::Continue:
			SelectTalkBox->TalkContinue();
			break;

		case ESubTalkType::Back:
			SelectTalkBox->TalkBack();
			break;

		case ESubTalkType::AcceptQuest:
			SelectTalkBox->TalkAcceptQuest();
			break;

		case ESubTalkType::Quit:
			SelectTalkBox->TalkQuit();
			break;

		default:
			break;
		}
	}
}

void USelectTalkInfo::UpdateSelectTalkInfo()
{
	if(TalkTypeImage)
	{
		// switch (TalkType)//根据TalkType设置Image的图像
		// {
		// default:
		// 	break;
		// }
		//TalkTypeImage->SetBrush();
		//TalkTypeImage->SetColorAndOpacity();
	}
	
	if(SelectTalkInfoText)
	{
		SelectTalkInfoText->SetText(FText::FromString(SelectTalkInfoString));
	}
}

#pragma region Get/Set

void USelectTalkInfo::SetTalkIndex(int Index)
{
	TalkIndex = Index;
}

void USelectTalkInfo::SetSubTalkType(ESubTalkType Type)
{
	SubTalkType = Type;
}

void USelectTalkInfo::SetSelectTalkBox(USelectTalkBox* const TalkBox)
{
	SelectTalkBox = TalkBox;	
}

void USelectTalkInfo::SetSelectTalkInfoString(FString TalkInfo)
{
	SelectTalkInfoString = TalkInfo;
}

#pragma endregion