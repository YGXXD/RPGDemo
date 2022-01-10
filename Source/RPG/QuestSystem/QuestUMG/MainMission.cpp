// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMission.h"

#include "MissionSubGoalItem.h"
#include "QuestBoard.h"
#include "QuestListItem.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "RPG/GameSystem/WidgetPool.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"


bool UMainMission::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	CurrentSelectedScrollBox = nullptr;
	
	//任务列表的滚动框
	AssignedQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName("AssignedQuestScrollBox"));
	if(AssignedQuestScrollBox)
	{
		AssignedQuestScrollBox->ClearChildren();
		AssignedQuestScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	CompleteQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName("CompleteQuestScrollBox"));
	if(CompleteQuestScrollBox)
	{
		CompleteQuestScrollBox->ClearChildren();
		CompleteQuestScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	FailedQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName("FailedQuestScrollBox"));
	if(FailedQuestScrollBox)
	{
		FailedQuestScrollBox->ClearChildren();
		FailedQuestScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	CanAcceptQuestScrollBox = Cast<UScrollBox>(GetWidgetFromName("CanAcceptQuestScrollBox"));
	if(CanAcceptQuestScrollBox)
	{
		CanAcceptQuestScrollBox->ClearChildren();
		CanAcceptQuestScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}

	//Description控件
	QuestDetailScrollBox = Cast<UScrollBox>(GetWidgetFromName("QuestDetailScrollBox"));
	if(QuestDetailScrollBox)
	{
		QuestDetailScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	DescriptionText = Cast<UTextBlock>(GetWidgetFromName("DescriptionText"));
	QuestCategoryImage = Cast<UImage>(GetWidgetFromName("QuestCategoryImage"));
	QuestCategoryText = Cast<UTextBlock>(GetWidgetFromName("QuestCategoryText"));
	RegionText = Cast<UTextBlock>(GetWidgetFromName("RegionText"));
	SuggestLevelText = Cast<UTextBlock>(GetWidgetFromName("SuggestLevelText"));

	//Description中的Reward奖励控件
	RewardBox = Cast<UVerticalBox>(GetWidgetFromName("RewardBox"));
	if(RewardBox)
	{
		RewardBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	MoneyHorizontalBox = Cast<UHorizontalBox>(GetWidgetFromName("MoneyHorizontalBox"));
	if(MoneyHorizontalBox)
	{
		MoneyHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	MoneyRewardText = Cast<UTextBlock>(GetWidgetFromName("MoneyRewardText"));
	
	ExpHorizontalBox = Cast<UHorizontalBox>(GetWidgetFromName("ExpHorizontalBox"));
	if(ExpHorizontalBox)
	{
		ExpHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	ExpRewardText = Cast<UTextBlock>(GetWidgetFromName("ExpRewardText"));
	
	PrestigeHorizontalBox = Cast<UHorizontalBox>(GetWidgetFromName("PrestigeHorizontalBox"));
	if(PrestigeHorizontalBox)
	{
		PrestigeHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	PrestigeRewardText = Cast<UTextBlock>(GetWidgetFromName("ExpRewardText"));

	//Description中的任务目标控件
	SubGoalInfoBox = Cast<UVerticalBox>(GetWidgetFromName("SubGoalInfoBox"));
	if(SubGoalInfoBox)
	{
		SubGoalInfoBox->SetVisibility(ESlateVisibility::Collapsed);
	}
	SubGoalVerticalBox = Cast<UVerticalBox>(GetWidgetFromName("SubGoalVerticalBox"));
	
	//任务列表按钮
	AssignedQuestButton = Cast<UButton>(GetWidgetFromName("AssignedQuestButton"));
	if(AssignedQuestButton)
	{
		AssignedQuestButton->OnClicked.AddDynamic(this,&UMainMission::OnAssignedQuestButtonClick);
	}
	CompleteQuestButton = Cast<UButton>(GetWidgetFromName("CompleteQuestButton"));
	if(CompleteQuestButton)
	{
		CompleteQuestButton->OnClicked.AddDynamic(this,&UMainMission::OnCompleteQuestButtonClick);
	}
	FailedQuestButton = Cast<UButton>(GetWidgetFromName("FailedQuestButton"));
	if(FailedQuestButton)
	{
		FailedQuestButton->OnClicked.AddDynamic(this,&UMainMission::OnFailedQuestButtonClick);
	}
	CanAcceptQuestButton = Cast<UButton>(GetWidgetFromName("CanAcceptQuestButton"));
	if(CanAcceptQuestButton)
	{
		CanAcceptQuestButton->OnClicked.AddDynamic(this,&UMainMission::OnCanAcceptQuestButtonClick);
	}

	//Description中的选择与取消按钮
	SelectQuestButton = Cast<UButton>(GetWidgetFromName("SelectQuestButton"));
	if(SelectQuestButton)
	{
		SelectQuestButton->OnClicked.AddDynamic(this,&UMainMission::OnSelectQuestButtonClick);
	}
	CancelQuestButton = Cast<UButton>(GetWidgetFromName("CancelQuestButton"));
	if(CancelQuestButton)
	{
		CancelQuestButton->OnClicked.AddDynamic(this,&UMainMission::OnCancelQuestButtonClick);
	}

	//获取动画
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	if(WidgetClass)
	{
		//AnimationMap的KeyValue:”RemoveQuestList_INST“和"AddQuestList_INST"
		for (int i = 0; i < WidgetClass->Animations.Num(); i++)
		{
			FString AnimationName;
			WidgetClass->Animations[i]->GetName(AnimationName);
			WidgetAnimationMap.Add(AnimationName,WidgetClass->Animations[i]);
		}
	}

	QuestListItemClass = LoadClass<UQuestListItem>(this,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Quest/WBP_QuestListItem.WBP_QuestListItem_C'"));
	if(QuestListItemClass)
	{
		CREATE_WIDGET_POOL(QuestListItemClass);
	}
	
	return true;
}

#pragma region Button绑定事件

void UMainMission::OnAssignedQuestButtonClick()
{
	const float RemoveDuration = RemoveQuestList();
	auto AddQuestListLambda = [this]()
	{
		if(CurrentSelectedScrollBox)
		{
			CurrentSelectedScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		}
		AssignedQuestScrollBox->SetVisibility(ESlateVisibility::Visible);
		CurrentSelectedScrollBox = AssignedQuestScrollBox;
		UpdateAssignedQuestScrollBox();
		AddQuestList();
	};
	const FTimerDelegate AddQuestListDelegate = FTimerDelegate::CreateLambda(AddQuestListLambda);
	GetWorld()->GetTimerManager().SetTimer(QuestListDelayHandle,AddQuestListDelegate,RemoveDuration,false);
}

void UMainMission::OnCompleteQuestButtonClick()
{
	const float RemoveDuration = RemoveQuestList();
	auto AddQuestListLambda = [this]()
	{
		if(CurrentSelectedScrollBox)
		{
			CurrentSelectedScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		}
		CompleteQuestScrollBox->SetVisibility(ESlateVisibility::Visible);
		CurrentSelectedScrollBox = CompleteQuestScrollBox;
		UpdateCompleteQuestScrollBox();
		AddQuestList();
	};
	const FTimerDelegate AddQuestListDelegate = FTimerDelegate::CreateLambda(AddQuestListLambda);
	GetWorld()->GetTimerManager().SetTimer(QuestListDelayHandle,AddQuestListDelegate,RemoveDuration,false);
}

void UMainMission::OnFailedQuestButtonClick()
{
	const float RemoveDuration = RemoveQuestList();
	auto AddQuestListLambda = [this]()
	{
		if(CurrentSelectedScrollBox)
		{
			CurrentSelectedScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		}
		FailedQuestScrollBox->SetVisibility(ESlateVisibility::Visible);
		CurrentSelectedScrollBox = FailedQuestScrollBox;
		UpdateFailedQuestScrollBox();
		AddQuestList();
	};
	const FTimerDelegate AddQuestListDelegate = FTimerDelegate::CreateLambda(AddQuestListLambda);
	GetWorld()->GetTimerManager().SetTimer(QuestListDelayHandle,AddQuestListDelegate,RemoveDuration,false);
}

void UMainMission::OnCanAcceptQuestButtonClick()
{
	const float RemoveDuration = RemoveQuestList();
	auto AddQuestListLambda = [this]()
	{
		if(CurrentSelectedScrollBox)
		{
			CurrentSelectedScrollBox->SetVisibility(ESlateVisibility::Collapsed);
		}
		CanAcceptQuestScrollBox->SetVisibility(ESlateVisibility::Visible);
		CurrentSelectedScrollBox = CanAcceptQuestScrollBox;
		UpdateCanAcceptScrollBox();
		AddQuestList();
	};
	const FTimerDelegate AddQuestListDelegate = FTimerDelegate::CreateLambda(AddQuestListLambda);
	GetWorld()->GetTimerManager().SetTimer(QuestListDelayHandle,AddQuestListDelegate,RemoveDuration,false);
}

void UMainMission::OnSelectQuestButtonClick()
{
	if(CurrentSelectQuestListItem)
	{
		QuestBoard->SetQuestToUpdate(CurrentSelectQuestListItem->GetQuest());
	}
}

void UMainMission::OnCancelQuestButtonClick()
{
	QuestBoard->HideQuestBoard();
}

#pragma endregion 

#pragma region Set/Get

void UMainMission::SetPlayer(ARPGameCharacter* const RPGameCharacter)
{
	Player = RPGameCharacter;
	if(Player)
	{
		Player->OnLevelUpUpdateQuest.AddDynamic(this,&UMainMission::UpdateQuestListSuggestLevel);
	}
}

void UMainMission::SetPlayerQuestBoard(UQuestBoard* const PlayerQuestBoard)
{
	QuestBoard = PlayerQuestBoard;
}

UQuestListItem* UMainMission::GetQuestListItem(const AMasterQuest* Quest)
{
	if(AssignedQuestItemMap.Contains(Quest))
	{
		return AssignedQuestItemMap[Quest];
	}
	return nullptr;
}

#pragma endregion

#pragma region 更新任务列表时，更新QuestDetail

void UMainMission::UpdateAssignedQuestScrollBox()
{
	if(!SelectedAssignedQuest && AssignedQuestList.Num())
	{
		SelectedAssignedQuest = AssignedQuestList[0];
	}
	else if(!AssignedQuestList.Num())
	{
		SelectedAssignedQuest = nullptr;
	}
	SelectQuestListItem(SelectedAssignedQuest);
}

void UMainMission::UpdateCompleteQuestScrollBox()
{
	if(!SelectedCompleteQuest && CompleteQuestList.Num())
	{
		SelectedCompleteQuest = CompleteQuestList[0];
	}
	else if(!CompleteQuestList.Num())
	{
		SelectedCompleteQuest = nullptr;
	}
	SelectQuestListItem(SelectedCompleteQuest);
}

void UMainMission::UpdateFailedQuestScrollBox()
{
	if(!SelectedFailedQuest && FailedQuestList.Num())
	{
		SelectedFailedQuest = FailedQuestList[0];
	}
	else if(!FailedQuestList.Num())
	{
		SelectedFailedQuest = nullptr;
	}
	SelectQuestListItem(SelectedFailedQuest);
}

void UMainMission::UpdateCanAcceptScrollBox()
{
	if(!SelectedCanAcceptQuest && CanAcceptQuestList.Num())
	{
		SelectedCanAcceptQuest = CanAcceptQuestList[0];
	}
	else if(!CanAcceptQuestList.Num())
	{
		SelectedCanAcceptQuest = nullptr;
	}
	SelectQuestListItem(SelectedCanAcceptQuest);
}

#pragma endregion 

#pragma region 更新任务细节面板

void UMainMission::UpdateQuestListSuggestLevel(int Level)
{
	for (int i = 0; i < AssignedQuestList.Num(); i++)
	{
		AssignedQuestList[i]->UpdateLevelColor(Level);
	}
	for (int i = 0; i < CompleteQuestList.Num(); i++)
	{
		CompleteQuestList[i]->UpdateLevelColor(Level);
	}
	for (int i = 0; i < FailedQuestList.Num(); i++)
	{
		FailedQuestList[i]->UpdateLevelColor(Level);
	}
	for (int i = 0; i < CanAcceptQuestList.Num(); i++)
	{
		CanAcceptQuestList[i]->UpdateLevelColor(Level);
	}
	if(SuggestLevelText && CurrentSelectQuestListItem)
	{
		SuggestLevelText->SetColorAndOpacity(CurrentSelectQuestListItem->GetLevelNumText()->ColorAndOpacity);
	}
}

void UMainMission::UpdateQuestDetail()
{
	if(CurrentSelectQuestListItem)
	{
		AMasterQuest* CurrentSelectedQuest = CurrentSelectQuestListItem->GetQuest();
		if(!CurrentSelectedQuest)
		{
			return;
		}
		
		QuestDetailScrollBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		const FQuestInfo QuestInfo = CurrentSelectedQuest->GetQuestInfo();

		//更新任务信息
		if(QuestNameText)
		{
			QuestNameText->SetText(CurrentSelectQuestListItem->GetQuestNameText()->GetText());
			QuestNameText->SetColorAndOpacity(CurrentSelectQuestListItem->GetQuestNameText()->ColorAndOpacity);
		}

		if(RegionText)
		{
			RegionText->SetText(CurrentSelectQuestListItem->GetRegionText()->GetText());
		}
		
		if(SuggestLevelText)
		{
			SuggestLevelText->SetColorAndOpacity(CurrentSelectQuestListItem->GetLevelNumText()->ColorAndOpacity);
			SuggestLevelText->SetText(FText::AsNumber(QuestInfo.SuggestLevel));
		}

		if(QuestCategoryImage)
		{
			QuestCategoryImage->SetBrushFromMaterial(CurrentSelectQuestListItem->GetQuestCategoryBorder()->GetDynamicMaterial());
		}

		if(QuestCategoryText)
		{
			QuestCategoryText->SetColorAndOpacity(CurrentSelectQuestListItem->GetQuestNameText()->ColorAndOpacity);
			switch (QuestInfo.Category)
			{
			case EQuestCategory::MainlineQuest:
				QuestCategoryText->SetText(FText::FromName(TEXT("主线任务")));
				break;

			case EQuestCategory::SidelineQuest:
				QuestCategoryText->SetText(FText::FromName(TEXT("支线任务")));
				break;

			case EQuestCategory::ChallengeQuest:
				QuestCategoryText->SetText(FText::FromName(TEXT("挑战任务")));
				break;
			
			default:
				QuestCategoryText->SetText(FText::FromName(TEXT("")));
				break;
			}
		}
		//更新Reward任务奖励
		if(RewardBox)
		{
			if(QuestInfo.Reward.Money + QuestInfo.Reward.Experience + QuestInfo.Reward.Prestige <= 0)
			{
				RewardBox->SetVisibility(ESlateVisibility::Collapsed);
			}
			else
			{
				RewardBox->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				if(QuestInfo.Reward.Money > 0)
				{
					if(MoneyHorizontalBox && MoneyRewardText)
					{
						MoneyHorizontalBox->SetVisibility(ESlateVisibility::Visible);
						MoneyRewardText->SetText(FText::FromString("+ " + FString::FromInt(QuestInfo.Reward.Money)));
					}
				}
				else
				{
					if(MoneyHorizontalBox)
					{
						MoneyHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
					}
				}

				if(QuestInfo.Reward.Experience > 0)
				{
					if(ExpHorizontalBox && ExpRewardText)
					{
						ExpHorizontalBox->SetVisibility(ESlateVisibility::Visible);
						ExpRewardText->SetText(FText::FromString("+ " + FString::FromInt(QuestInfo.Reward.Experience)));
					}
				}
				else
				{
					if(ExpHorizontalBox)
					{
						ExpHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
					}
				}

				if(QuestInfo.Reward.Prestige > 0)
				{
					if(PrestigeHorizontalBox && PrestigeRewardText)
					{
						PrestigeHorizontalBox->SetVisibility(ESlateVisibility::Visible);
						PrestigeRewardText->SetText(FText::FromString("+ " + FString::FromInt(QuestInfo.Reward.Prestige)));
					}
				}
				else
				{
					if(PrestigeHorizontalBox)
					{
						PrestigeHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
					}
				}
			}
		}
		
		//更新任务描述
		if(DescriptionText)
		{
			DescriptionText->SetText(FText::FromString(QuestInfo.Description));
		}
		
		//更新SubGoalList
		UpdateSubGoalInfoBox();
	}
	else
	{
		QuestDetailScrollBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UMainMission::UpdateSubGoalInfoBox()
{
	if(SubGoalVerticalBox && SubGoalInfoBox)
	{
		SubGoalVerticalBox->ClearChildren();
		if(CurrentSelectQuestListItem)
		{
			const TArray<UMissionSubGoalItem*> MissionSubGoalList = CurrentSelectQuestListItem->GetMissionSubGoalList();
			if(MissionSubGoalList.Num())
			{
				for (int i = 0; i < MissionSubGoalList.Num(); i++)
				{
					SubGoalVerticalBox->AddChildToVerticalBox(MissionSubGoalList[i]);
				}
				SubGoalInfoBox->SetVisibility(ESlateVisibility::Visible);
			}
			else
			{
				SubGoalInfoBox->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
	}
}

#pragma endregion 

#pragma region 动画效果

float UMainMission::AddQuestList()
{
	float AnimDuration = 0;
	if (WidgetAnimationMap.Contains("AddQuestList_INST"))
	{
		AnimDuration = WidgetAnimationMap["RemoveQuestList_INST"]->GetEndTime();
		PlayAnimation(WidgetAnimationMap["AddQuestList_INST"]);
	}
	return AnimDuration;
}

float UMainMission::RemoveQuestList()
{
	float AnimDuration = 0;
	if (WidgetAnimationMap.Contains("RemoveQuestList_INST"))
	{
		AnimDuration = WidgetAnimationMap["RemoveQuestList_INST"]->GetEndTime();
		PlayAnimation(WidgetAnimationMap["RemoveQuestList_INST"]);
	}
	return AnimDuration;
}

#pragma endregion

#pragma region 接受任务，交互接口

void UMainMission::UpdateAssignedQuest(TArray<AMasterQuest*>& QuestList)
{
	if(AssignedQuestScrollBox)
	{
		// if(!AssignedQuestScrollBox->HasAnyChildren())
		// {
		// 	QuestBoard->SetQuestToUpdate(Quest);
		// }
		WIDGET_POOL->RemoveWidgetAllChildren<UQuestListItem>(QuestListItemClass,AssignedQuestScrollBox);
		AssignedQuestList.Empty();
		for (AMasterQuest* Quest : QuestList)
		{
			if(QuestListItemClass)
			{
				UQuestListItem* QuestListItem = WIDGET_POOL->GetUserWidgetFromUMGPool<UQuestListItem>(QuestListItemClass,GetWorld());
				QuestListItem->SetUpQuest(Quest);
				QuestListItem->UpdateLevelColor(Player->GetCurrentLevel());
				QuestListItem->SetMainMission(this);
				AssignedQuestList.Add(QuestListItem);
				AssignedQuestScrollBox->AddChild(QuestListItem);
				AssignedQuestItemMap.Add(Quest,QuestListItem);
			}
		}
	}
}

void UMainMission::UpdateCompleteQuest(TArray<AMasterQuest*>& QuestList)
{
	if(CompleteQuestScrollBox)
	{
		WIDGET_POOL->RemoveWidgetAllChildren<UQuestListItem>(QuestListItemClass,CompleteQuestScrollBox);
		CompleteQuestList.Empty();
		for (AMasterQuest* Quest : QuestList)
		{
			if(QuestListItemClass)
			{
				UQuestListItem* QuestListItem = WIDGET_POOL->GetUserWidgetFromUMGPool<UQuestListItem>(QuestListItemClass,GetWorld());
				QuestListItem->SetUpQuest(Quest);
				QuestListItem->UpdateLevelColor(Player->GetCurrentLevel());
				QuestListItem->SetMainMission(this);
				CompleteQuestList.Add(QuestListItem);
				CompleteQuestScrollBox->AddChild(QuestListItem);
			}
		}
	}
}

void UMainMission::SelectQuestListItem(UQuestListItem* QuestListItem)
{
	if(CurrentSelectQuestListItem)
	{
		CurrentSelectQuestListItem->GetQuestButton()->SetIsEnabled(true);
	}
	if(QuestListItem)
	{
		QuestListItem->GetQuestButton()->SetIsEnabled(false);
	}
	CurrentSelectQuestListItem = QuestListItem;
	UpdateQuestDetail();
}

void UMainMission::UpdateAllSubGoalStateInQuest(const AMasterQuest* Quest)
{
	if(AssignedQuestItemMap.Contains(Quest))
	{
		AssignedQuestItemMap[Quest]->UpdateMissionSubGoalListState();
	}
	if(QuestBoard && QuestBoard->GetAssignedQuest() == Quest)
	{
		QuestBoard->UpdateSubGoalBoardListState();
	}
}

void UMainMission::UpdateQuestFollowingSubGoal(AMasterQuest* Quest)
{
	if(AssignedQuestItemMap.Contains(Quest))
	{
		UQuestListItem* QuestListItem = AssignedQuestItemMap[Quest];
		QuestListItem->SpawnSubGoalList();
		if(AssignedQuestItemMap[Quest] == CurrentSelectQuestListItem)
		{
			UpdateSubGoalInfoBox();
		}
	}
	if(QuestBoard && QuestBoard->GetAssignedQuest() == Quest)
	{
		//UpdateQuestBoard函数中，有任务会更新，没有任务则会隐藏QuestBoard
		QuestBoard->UpdateQuestBoard();
	}
}

void UMainMission::UpdateCurrentSelectedScrollBox()
{
	//当前选中页面为什么页面就更新什么页面
	if (CurrentSelectedScrollBox == AssignedQuestScrollBox)
	{
		UpdateAssignedQuestScrollBox();
	}
	else if (CurrentSelectedScrollBox == CompleteQuestScrollBox)
	{
		UpdateCompleteQuestScrollBox();
	}
}

void UMainMission::UpdateSubGoalItemInfo(const AMasterQuest* Quest, int CurrenSubGoalID)
{
	if(AssignedQuestItemMap.Contains(Quest))
	{
		UQuestListItem* QuestListItem = AssignedQuestItemMap[Quest];
		QuestListItem->UpdateMissionSubGoalItemInfo(CurrenSubGoalID);
		if(QuestBoard && QuestBoard->GetAssignedQuest() == Quest)
		{
			QuestBoard->UpdateSubGoalBoardItemInfo(CurrenSubGoalID);
		}
	}
}

#pragma endregion