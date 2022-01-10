// Fill out your copyright notice in the Description page of Project Settings.


#include "MasterQuest.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/QuestSystem/QuestUMG/QuestBoard.h"

#pragma region 结构体运算符重载

bool FSubGoalTargetTransform::operator==(const FSubGoalTargetTransform& QuestTargetTransform) const
{
	if(bHasTargetLocation == QuestTargetTransform.bHasTargetLocation && TargetLocation == QuestTargetTransform.TargetLocation)
		return true;

	return false;
}

bool FSubGoalInfo::operator==(const FSubGoalInfo& SubGoalInfo) const
{
	if (Type == SubGoalInfo.Type &&
		//bIsCustomSubGoal == SubGoalInfo.bIsCustomSubGoal &&
		CustomSubGoalText == SubGoalInfo.CustomSubGoalText &&
		TargetID == SubGoalInfo.TargetID &&
		TargetName == SubGoalInfo.TargetName &&
		TargetActorClass == SubGoalInfo.TargetActorClass &&
		HuntAmount == SubGoalInfo.HuntAmount &&
		TargetTransform == SubGoalInfo.TargetTransform &&
		bCanUpdateQuestDescription == SubGoalInfo.bCanUpdateQuestDescription &&
		UpdateDescription == SubGoalInfo.UpdateDescription &&
		FollowingSubGoalIDList == SubGoalInfo.FollowingSubGoalIDList
	)
	{
		return true;
	}
	
	return false;
}

#pragma endregion

AMasterQuest::AMasterQuest(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMasterQuest::BeginPlay()
{
	Super::BeginPlay();
	SetUpStartSubGoalList();
}

#pragma region Set/Get

TArray<int>& AMasterQuest::GetCurrentSubGoalTargetNumList()
{
	return CurrentSubGoalTargetNumList;
}

FQuestInfo& AMasterQuest::GetQuestInfo()
{
	return QuestInfo;
}

TArray<FSubGoalInfo>& AMasterQuest::GetCurrentSubGoalInfoList()
{
	return CurrentSubGoalInfoList;
}

TArray<FSubGoalInfo>& AMasterQuest::GetCompleteSubGoalInfoList()
{
	return CompleteSubGoalInfoList;
}

TArray<int>& AMasterQuest::GetCurrentSubGoalIDList()
{
	return CurrentSubGoalInQuestIDList;
}

#pragma endregion

#pragma region 设置和更新任务列表

void AMasterQuest::UpdateSubGoalInfoList()
{
	CurrentSubGoalInfoList.Empty();
	for(int i=0; i<CurrentSubGoalInQuestIDList.Num(); i++)
	{
		FSubGoalInfo SubGoalInfo = QuestInfo.SubGoalInfoList[CurrentSubGoalInQuestIDList[i]];
		CurrentSubGoalInfoList.Add(SubGoalInfo);
	}
}

void AMasterQuest::SetUpStartSubGoalList()
{
	CurrentSubGoalInQuestIDList.Empty();
	CurrentSubGoalTargetNumList.Empty();
	CurrentSubGoalInQuestIDList = StartSubGoalIDList;
	for (int i = 0; i < CurrentSubGoalInQuestIDList.Num(); i++)
	{
		CurrentSubGoalTargetNumList.Add(0);
	}
	UpdateSubGoalInfoList();
}

bool AMasterQuest::StartNextSubGoal()
{
	int IndexOfMaxValue;
	int MaxValue;
	UKismetMathLibrary::MaxOfIntArray(CurrentSubGoalInQuestIDList,IndexOfMaxValue,MaxValue);
	if(MaxValue + 1 < QuestInfo.SubGoalInfoList.Num())
	{
		CurrentSubGoalInQuestIDList.Empty();
		CurrentSubGoalInQuestIDList.Add(MaxValue + 1);
		UpdateSubGoalInfoList();
		return true;
	}
	return false;
}

void AMasterQuest::AddTargetNum(int TargetNumListID)
{
	if(TargetNumListID < CurrentSubGoalTargetNumList.Num() && TargetNumListID >= 0)
	{
		CurrentSubGoalTargetNumList[TargetNumListID] += 1;
		AddTargetNumRecall(TargetNumListID);
		if(CurrentSubGoalTargetNumList[TargetNumListID] >= CurrentSubGoalInfoList[TargetNumListID].HuntAmount)
		{
			const int SubGoalIDInQuest = FindSubGoalInfoIndex(CurrentSubGoalInfoList[TargetNumListID]);
			CompleteSubGoal(SubGoalIDInQuest);
		}
	}
}

void AMasterQuest::CompleteSubGoal(int SubGoalIndex)
{
	if(CurrentSubGoalInQuestIDList.Contains(SubGoalIndex))
	{
		const FSubGoalInfo CompleteSubGoalInfo = QuestInfo.SubGoalInfoList[SubGoalIndex];
		CurrentSubGoalInQuestIDList.Remove(SubGoalIndex);//当先任务索引表移除索引
		CompleteSubGoalInfoList.Add(CompleteSubGoalInfo);//加入到完成任务表里面
		CompleteSubGoalReCall();//更新完成任务的状态
		if(!CurrentSubGoalInQuestIDList.Num())//如果索引表没有索引了,从完成的任务里找到后续任务
		{
			UpdateFollowingSubGoal();
		}
	}
}

void AMasterQuest::UpdateFollowingSubGoal()
{
	CurrentSubGoalInfoList.Empty();
	CurrentSubGoalInQuestIDList.Empty();
	CurrentSubGoalTargetNumList.Empty();
	while (CompleteSubGoalInfoList.Num() != 0)
	{
		//判断是否还有后续任务
		if(CompleteSubGoalInfoList[0].FollowingSubGoalIDList.Num() != 0)
		{
			//还有有序任务则更新
			const FSubGoalInfo CompleteSubGoalInfo = CompleteSubGoalInfoList[0];
			const int IDListLength = CompleteSubGoalInfo.FollowingSubGoalIDList.Num();
			for (int j = 0; j < IDListLength; j++)
			{
				CurrentSubGoalInQuestIDList.Add(CompleteSubGoalInfo.FollowingSubGoalIDList[j]);
				const FSubGoalInfo FollowingSubGoal = QuestInfo.SubGoalInfoList[CompleteSubGoalInfo.FollowingSubGoalIDList[j]];
				CurrentSubGoalInfoList.Add(FollowingSubGoal);
				CurrentSubGoalTargetNumList.Add(0);
			}
			CompleteSubGoalInfoList.RemoveAt(0);
			break;
		}
		CompleteSubGoalInfoList.RemoveAt(0);
	}
	CompleteCurrentSubGoalReCall();
}

void AMasterQuest::AddTargetNumRecall(int TargetNumListIndex)
{
	OnTargetNumAddUpdateSubGoalInfo.Broadcast(this,TargetNumListIndex);	
}

void AMasterQuest::CompleteCurrentSubGoalReCall()
{
	OnCompleteCurrentSubGoalList.Broadcast(this);
}

void AMasterQuest::CompleteSubGoalReCall()
{
	OnCompleteSubGoal.Broadcast(this);
}

#pragma endregion 

int AMasterQuest::FindSubGoalInfoIndex(const FSubGoalInfo& SubGoalInfo)
{
	for(int i=0; i<QuestInfo.SubGoalInfoList.Num(); i++)
	{
		if(SubGoalInfo == QuestInfo.SubGoalInfoList[i])
		{
			return i;
		}
	}
	return -1;
}