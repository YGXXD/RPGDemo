// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestComponent.h"

#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/Player/ControllerUMG/MiniMap.h"
#include "RPG/Player/ControllerUMG/PlayerControllerMainUI.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "RPG/QuestSystem/QuestUMG/MainMission.h"
#include "RPG/QuestSystem/QuestUMG/QuestBoard.h"
#include "RPG/LoadSystem/SaveData/RPGSaveGame.h"

UQuestComponent::UQuestComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bHasSubGoalTargetLocation = false;
}

void UQuestComponent::SetPlayerAndMainUI(ARPGameCharacter* const RPGameCharacter, UPlayerControllerMainUI* const MainUI)
{
	Player = RPGameCharacter;
	PCMainUI = MainUI;
	if(PCMainUI && Player)
	{
		PCMainUI->GetMainMission()->SetPlayer(Player);
		PCMainUI->GetMainMission()->SetPlayerQuestBoard(PCMainUI->GetQuestBoard());
		PCMainUI->GetQuestBoard()->OnMiniMapTargetShow.AddDynamic(this,&UQuestComponent::MiniMapTargetShow);
	}
}

#pragma region 任务交互更新逻辑

void UQuestComponent::AddQuest(TSubclassOf<AMasterQuest> QuestClass)
{
	//QuestListItem需要更新等级颜色
	if(!AllQuestClassList.Contains(QuestClass) && QuestClass)
	{
		AllQuestClassList.Add(QuestClass);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AMasterQuest* Quest = GetWorld()->SpawnActor<AMasterQuest>(QuestClass,SpawnParams);
		UE_LOG(LogTemp,Warning,TEXT("%f,%f,%f"),Quest->GetActorLocation().X,Quest->GetActorLocation().Y,Quest->GetActorLocation().Z)
		AssignedQuestList.Add(Quest);
		if(PCMainUI)
		{
			PCMainUI->GetMainMission()->UpdateAssignedQuest(AssignedQuestList);
			Quest->OnCompleteSubGoal.AddDynamic(PCMainUI->GetMainMission(),&UMainMission::UpdateAllSubGoalStateInQuest);
			Quest->OnCompleteCurrentSubGoalList.AddDynamic(PCMainUI->GetMainMission(),&UMainMission::UpdateQuestFollowingSubGoal);
			//更新单项UI文本的逻辑
			Quest->OnTargetNumAddUpdateSubGoalInfo.AddDynamic(PCMainUI->GetMainMission(),&UMainMission::UpdateSubGoalItemInfo);
		}
	}
}

void UQuestComponent::OnSubGoalTargetComplete(TSubclassOf<AActor> TargetClass)
{
	for (const auto& Elem : AssignedQuestList)
	{
		const TArray<FSubGoalInfo> SubGoalInfoList = Elem->GetCurrentSubGoalInfoList();
		for (int i = 0; i < SubGoalInfoList.Num(); i++)
		{
			const int SubGoalIndex = Elem->FindSubGoalInfoIndex(SubGoalInfoList[i]);
			if(TargetClass == SubGoalInfoList[i].TargetActorClass && Elem->GetCurrentSubGoalIDList().Contains(SubGoalIndex))
			{
				Elem->AddTargetNum(i);
			}
		}
	}
}

bool UQuestComponent::TryCompleteQuest(TSubclassOf<AMasterQuest> QuestClass)
{
	for (int i = 0; i < AssignedQuestList.Num(); i++)
	{
		if(AssignedQuestList[i]->GetClass() == QuestClass)
		{
			AMasterQuest* Quest = AssignedQuestList[i];
			//判断索引表是否还有，没有的话任务完成
			if(!Quest->GetCurrentSubGoalIDList().Num())
			{
				Player->ModifyAttribute(EAttributeType::Experience,Quest->GetQuestInfo().Reward.Experience);
				Player->ModifyAttribute(EAttributeType::Money,Quest->GetQuestInfo().Reward.Money);
				AssignedQuestList.RemoveAt(i);	
				CompleteQuestList.Add(Quest);
				if(PCMainUI)
				{
					PCMainUI->GetMainMission()->UpdateAssignedQuest(AssignedQuestList);
					PCMainUI->GetMainMission()->UpdateCompleteQuest(CompleteQuestList);
					PCMainUI->GetMainMission()->UpdateCurrentSelectedScrollBox();
				}
				return true;
			}
		}
	}
	return false;
}

void UQuestComponent::CompleteSubGoal(int SubGoalIndex)
{
	for (const auto& Elem : AssignedQuestList)
	{
		Elem->CompleteSubGoal(SubGoalIndex);
	}
}

#pragma endregion 

#pragma region MiniMap的任务目标信息更新

float UQuestComponent::UpdateTargetDistance()
{
	const FVector PlayerLocation = Player->GetActorLocation();
	const FVector2D Player2DLocation(PlayerLocation.X,PlayerLocation.Y);
	const FVector2D Target2DLocation(CurrentTargetLocation.X,CurrentTargetLocation.Y);
	const float DistanceValue = UKismetMathLibrary::Distance2D(Player2DLocation,Target2DLocation)/100;
	if(PCMainUI)
	{
		PCMainUI->GetMiniMap()->SetDistance(DistanceValue);
	}
	return DistanceValue;
}

void UQuestComponent::UpdateTargetDirection()
{
	const FVector PlayerLocation = Player->GetActorLocation();
	const FRotator LookRotator = UKismetMathLibrary::FindLookAtRotation(PlayerLocation,CurrentTargetLocation);
	if(PCMainUI)
	{
		PCMainUI->GetMiniMap()->SetDirection(LookRotator.Yaw);
	}
}

void UQuestComponent::UpdateTargetOnMove()
{
	if(bHasSubGoalTargetLocation)
	{
		UpdateTargetDirection();
		const float Distance = UpdateTargetDistance();
		if(PCMainUI)
		{
			if(Distance < 5)
			{
				PCMainUI->GetMiniMap()->GetTargetDirectionImage()->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				PCMainUI->GetMiniMap()->GetTargetDirectionImage()->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
}

void UQuestComponent::MiniMapTargetShow(bool bHasTargetLocation,FVector TargetLocation)
{
	CurrentTargetLocation = TargetLocation;
	bHasSubGoalTargetLocation = bHasTargetLocation;
	if(bHasSubGoalTargetLocation)
	{
		UpdateTargetDirection();
		UpdateTargetDistance();
		if(PCMainUI)
		{
			PCMainUI->GetMiniMap()->ShowTargetDistanceOverlay();
		}
	}
	else
	{
		if(PCMainUI)
		{
			PCMainUI->GetMiniMap()->HideTargetDistanceOverlay();
		}
	}
}

#pragma endregion

#pragma region 保存任务

void UQuestComponent::SaveQuest() const
{
	SAVE_DATA->SavePlayerQuest(AssignedQuestList,CompleteQuestList);
}

void UQuestComponent::LoadQuest(int Index)
{
	const FSaveQuestList SaveQuestList = SAVE_DATA->GetSavedQuestList(Index);
	
	TArray<TSubclassOf<AMasterQuest>> CompleteQuestClassList = SaveQuestList.CompleteQuestList;
	for (int i = 0; i < CompleteQuestClassList.Num(); i++)
	{
		if(!AllQuestClassList.Contains(CompleteQuestClassList[i]) && CompleteQuestClassList[i])
		{
			AllQuestClassList.Add(CompleteQuestClassList[i]);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AMasterQuest* Quest = GetWorld()->SpawnActor<AMasterQuest>(CompleteQuestClassList[i],SpawnParams);
			Quest->GetCurrentSubGoalInfoList().Empty();
			Quest->GetCurrentSubGoalIDList().Empty();
			CompleteQuestList.Add(Quest);
		}
	}
	
	TArray<TSubclassOf<AMasterQuest>> AssignedQuestClassList = SaveQuestList.AssignedQuestList;
	for (int i = 0; i < AssignedQuestClassList.Num(); i++)
	{
		if(!AllQuestClassList.Contains(AssignedQuestClassList[i]) && AssignedQuestClassList[i])
		{
			AllQuestClassList.Add(AssignedQuestClassList[i]);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AMasterQuest* Quest = GetWorld()->SpawnActor<AMasterQuest>(AssignedQuestClassList[i],SpawnParams);
			const FAssignedQuestInfo AssignedQuestInfo = SaveQuestList.AssignedQuestInfoList[i];
			Quest->GetCurrentSubGoalTargetNumList() = AssignedQuestInfo.CurrentSubGoalTargetNumList;
			Quest->GetCurrentSubGoalIDList() = AssignedQuestInfo.CurrentSubGoalInQuestIDList;
			Quest->GetCurrentSubGoalInfoList() = AssignedQuestInfo.CurrentSubGoalInfoList;
			Quest->GetCompleteSubGoalInfoList() = AssignedQuestInfo.CompleteSubGoalInfoList;
			AssignedQuestList.Add(Quest);
			if(PCMainUI)
			{
				Quest->OnCompleteSubGoal.AddDynamic(PCMainUI->GetMainMission(),&UMainMission::UpdateAllSubGoalStateInQuest);
				Quest->OnCompleteCurrentSubGoalList.AddDynamic(PCMainUI->GetMainMission(),&UMainMission::UpdateQuestFollowingSubGoal);
				Quest->OnTargetNumAddUpdateSubGoalInfo.AddDynamic(PCMainUI->GetMainMission(),&UMainMission::UpdateSubGoalItemInfo);
			}
		}
	}
	if(PCMainUI)
	{
		PCMainUI->GetMainMission()->UpdateCompleteQuest(CompleteQuestList);
		PCMainUI->GetMainMission()->UpdateAssignedQuest(AssignedQuestList);
	}
}

#pragma endregion