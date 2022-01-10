// Fill out your copyright notice in the Description page of Project Settings.


#include "SubGoalBoard.h"

#include "QuestBoard.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


bool USubGoalBoard::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	MainBorder = Cast<UBorder>(GetWidgetFromName("MainBorder"));
	if(MainBorder)
	{
		MainBorder->SetContentColorAndOpacity(FLinearColor(1,1,1,0));
	}
	
	DoneImage = Cast<UImage>(GetWidgetFromName("DoneImage"));
	
	SubGoalText = Cast<UTextBlock>(GetWidgetFromName("SubGoalText"));
	if(SubGoalText)
	{
		SubGoalText->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	SelectButton = Cast<UButton>(GetWidgetFromName("SelectButton"));
	if(SelectButton)
	{
		SelectButton->OnClicked.AddDynamic(this,&USubGoalBoard::OnSelectButtonOnclick);
	}
	
	//获取动画
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	if(WidgetClass && WidgetClass->Animations.Num())
	{
		ShowSubGoalBoardAnimation = WidgetClass->Animations[0];
	}
	
	return true;
}

#pragma region Set/Get

void USubGoalBoard::SetQuestBoard(UQuestBoard* const Board)
{
	QuestBoard = Board;
}

void USubGoalBoard::SetQuestSubGoalID(int ID)
{
	QuestSubGoalID = ID;
}

int USubGoalBoard::GetQuestSubGoalID() const
{
	return QuestSubGoalID;
}

FText USubGoalBoard::GetSubGoalInfoText() const
{
	return SubGoalText->GetText();
}

UButton* USubGoalBoard::GetSelectButton() const
{
	return SelectButton;
}

#pragma endregion 

#pragma region 更新SubGoalBoard的信息

void USubGoalBoard::OnSelectButtonOnclick()
{
	if(QuestBoard)
	{
		QuestBoard->SelectSubGoalBoard(this);
	}
}

void USubGoalBoard::UpdateSubGoalBoard(AMasterQuest* AssignedQuest,int CurrentSubGoalID)
{
	const FSubGoalInfo SubGoalInfo = AssignedQuest->GetCurrentSubGoalInfoList()[CurrentSubGoalID];
	FString SubGoalDescribe = "";
	switch (SubGoalInfo.Type)
	{
	case ESubGoalType::Custom:
		SubGoalDescribe = SubGoalInfo.CustomSubGoalText;
		break;
	
	case ESubGoalType::Hunt:
		SubGoalDescribe =
			TEXT("狩猎") + SubGoalInfo.TargetName + ":" +
			FString::FromInt(AssignedQuest->GetCurrentSubGoalTargetNumList()[CurrentSubGoalID]) + "/" +
			FString::FromInt(SubGoalInfo.HuntAmount);
		break;
	
	case ESubGoalType::Explore:
		SubGoalDescribe =
			TEXT("寻找") + SubGoalInfo.TargetName + ":" +
			FString::FromInt(AssignedQuest->GetCurrentSubGoalTargetNumList()[CurrentSubGoalID]) + "/" +
			FString::FromInt(SubGoalInfo.HuntAmount);
		break;
	
	case ESubGoalType::Talk:
		SubGoalDescribe =
			TEXT("与") + SubGoalInfo.TargetName + TEXT("交谈");
		break;
	
	default:
		break;
	}
	
	SubGoalText->SetText(FText::FromString(SubGoalDescribe));
}

void USubGoalBoard::UpdateSubGoalState(AMasterQuest* AssignedQuest)
{
	if(DoneImage && SelectButton)
	{
		const TArray<int> CurrentSubGoalIDList = AssignedQuest->GetCurrentSubGoalIDList();
		if(CurrentSubGoalIDList.Contains(QuestSubGoalID))
		{
			DoneImage->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			DoneImage->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

#pragma endregion 

void USubGoalBoard::ShowSubGoalBoard()
{
	PlayAnimation(ShowSubGoalBoardAnimation);
}

void USubGoalBoard::HideSubGoalBoard()
{
	PlayAnimation(ShowSubGoalBoardAnimation,0,1,EUMGSequencePlayMode::Reverse,2);
}

