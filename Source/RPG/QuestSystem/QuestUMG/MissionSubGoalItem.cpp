// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionSubGoalItem.h"

#include "SubGoalBoard.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

bool UMissionSubGoalItem::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	SubGoalStateImage = Cast<UImage>(GetWidgetFromName("SubGoalStateImage"));
	SubGoalText = Cast<UTextBlock>(GetWidgetFromName("SubGoalText"));
	
	return true;
}

#pragma region Set/Get

void UMissionSubGoalItem::SetQuestSubGoalID(int ID)
{
	QuestSubGoalID = ID;
}

#pragma endregion 

void UMissionSubGoalItem::UpdateMissionSubGoalItem(AMasterQuest* AssignedQuest,int CurrentSubGoalID)
{
	const FSubGoalInfo SubGoalInfo = AssignedQuest->GetCurrentSubGoalInfoList()[CurrentSubGoalID];
	if(SubGoalText)
	{
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
}

void UMissionSubGoalItem::UpdateSubGoalItemState(AMasterQuest* AssignedQuest)
{
	if(SubGoalStateImage)
	{
		const TArray<int> CurrentSubGoalIDList = AssignedQuest->GetCurrentSubGoalIDList();
		if(CurrentSubGoalIDList.Contains(QuestSubGoalID))
		{
			SubGoalStateImage->SetColorAndOpacity(FLinearColor(1,1,1));
		}
		else
		{
			SubGoalStateImage->SetColorAndOpacity(FLinearColor(1,1,0));
		}
	}
}
