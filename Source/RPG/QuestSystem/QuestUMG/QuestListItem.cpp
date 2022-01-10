// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestListItem.h"

#include "MainMission.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "RPG/GameSystem/WidgetPool.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "RPG/QuestSystem/QuestUMG/MissionSubGoalItem.h"

bool UQuestListItem::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	QuestCategoryBorder = Cast<UBorder>(GetWidgetFromName("QuestCategoryBorder"));
	ItemFrame = Cast<UBorder>(GetWidgetFromName("ItemFrame"));
	QuestNameText = Cast<UTextBlock>(GetWidgetFromName("QuestNameText"));
	LevelNumText = Cast<UTextBlock>(GetWidgetFromName("LevelNumText"));
	RegionText = Cast<UTextBlock>(GetWidgetFromName("RegionText"));
	QuestButton = Cast<UButton>(GetWidgetFromName("QuestButton"));
	if(QuestButton)
	{
		QuestButton->OnClicked.AddDynamic(this,&UQuestListItem::OnQuestButtonClick);
	}

	MissionSubGoalItemClass =
		LoadClass<UMissionSubGoalItem>(this,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Quest/WBP_MissionSubGoalItem.WBP_MissionSubGoalItem_C'"));
	if(MissionSubGoalItemClass)
	{
		CREATE_WIDGET_POOL(MissionSubGoalItemClass);
	}
	
	return true;
}

void UQuestListItem::OnQuestButtonClick()
{
	if(MainMission)
	{
		MainMission->SelectQuestListItem(this);
	}
}

void UQuestListItem::SpawnSubGoalList()
{
	WIDGET_POOL->EmptyWidgetList<UMissionSubGoalItem>(MissionSubGoalItemClass,MissionSubGoalList);
	const TArray<FSubGoalInfo> CurrentSubGoalInfoList = AssignedQuest->GetCurrentSubGoalInfoList();
	for (int i = 0; i < CurrentSubGoalInfoList.Num(); i++)
	{
		if(MissionSubGoalItemClass)
		{
			UMissionSubGoalItem* SubGoalItem = WIDGET_POOL->GetUserWidgetFromUMGPool<UMissionSubGoalItem>(MissionSubGoalItemClass,GetWorld());
			if(SubGoalItem)
			{
				SubGoalItem->SetQuestSubGoalID(AssignedQuest->FindSubGoalInfoIndex(CurrentSubGoalInfoList[i]));
				SubGoalItem->UpdateMissionSubGoalItem(AssignedQuest,i);
				SubGoalItem->UpdateSubGoalItemState(AssignedQuest);
				MissionSubGoalList.Add(SubGoalItem);
			}
		}
	}
}

void UQuestListItem::UpdateQuestListItem()
{
	const FQuestInfo QuestInfo = AssignedQuest->GetQuestInfo();
	FString QuestName = QuestInfo.Name;
	if(QuestInfo.Name.Len() > 13)
	{
		QuestName = QuestInfo.Name.LeftChop(12) + "...";
	}
	if(QuestNameText && ItemFrame)
	{
		FLinearColor CategoryColor;
		UMaterialInstance* QuestCategoryIcon = nullptr;
		switch (QuestInfo.Category)
		{
		case EQuestCategory::MainlineQuest:
			CategoryColor = FLinearColor(1,1,0);
			QuestCategoryIcon = LoadObject<UMaterialInstance>(this,TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_QuestMain.MI_QuestMain'"));
			break;

		case EQuestCategory::SidelineQuest:
			CategoryColor = FLinearColor(1,1,1);
			QuestCategoryIcon = LoadObject<UMaterialInstance>(this,TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_QuestSide.MI_QuestSide'"));
			break;

		case EQuestCategory::ChallengeQuest:
			CategoryColor = FLinearColor(1,0,0);
			QuestCategoryIcon = LoadObject<UMaterialInstance>(this,TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_QuestrChallenge.MI_QuestrChallenge'"));
			break;
			
		default:
			break;
		}
		QuestCategoryBorder->SetBrushFromMaterial(QuestCategoryIcon);
		QuestNameText->SetText(FText::FromString(QuestName));
		QuestNameText->SetColorAndOpacity(FSlateColor(CategoryColor));
		ItemFrame->SetBrushColor(CategoryColor);
	}
	if(LevelNumText)
	{
		LevelNumText->SetText(FText::FromString(TEXT("建议等级:") + FString::FromInt(QuestInfo.SuggestLevel)));
	}
	if(RegionText)
	{
		switch (QuestInfo.Region)
		{
		case EQuestRegion::IceLand:
			RegionText->SetText(FText::FromName(TEXT("区域:冰岛")));
			break;

		case EQuestRegion::Grotto:
			RegionText->SetText(FText::FromName(TEXT("区域:洞穴")));
			break;

		case EQuestRegion::LavaLand:
			RegionText->SetText(FText::FromName(TEXT("区域:岩浆岛")));
			break;
			
		default:
			RegionText->SetText(FText::FromString(TEXT("区域:")));
			break;
		}
	}
}

void UQuestListItem::UpdateLevelColor(int Level)
{
	const FQuestInfo QuestInfo = AssignedQuest->GetQuestInfo();
	if(LevelNumText)
	{
		if(QuestInfo.SuggestLevel > Level)
		{
			LevelNumText->SetColorAndOpacity(FSlateColor(FLinearColor(1,0,0)));
		}
		else if(QuestInfo.SuggestLevel < Level - 3)
		{
			LevelNumText->SetColorAndOpacity(FSlateColor(FLinearColor(1,1,1)));
		}
		else
		{
			LevelNumText->SetColorAndOpacity(FSlateColor(FLinearColor(1,1,0)));
		}
	}
}

void UQuestListItem::UpdateMissionSubGoalListState()
{
	for (int i = 0; i < MissionSubGoalList.Num(); i++)
	{
		MissionSubGoalList[i]->UpdateSubGoalItemState(AssignedQuest);
	}
}

void UQuestListItem::UpdateMissionSubGoalItemInfo(int MissionSubGoalItemID)
{
	if(MissionSubGoalItemID >= 0 && MissionSubGoalItemID < MissionSubGoalList.Num())
	{
		MissionSubGoalList[MissionSubGoalItemID]->UpdateMissionSubGoalItem(AssignedQuest,MissionSubGoalItemID);
	}
}

#pragma region Set/Get

void UQuestListItem::SetMainMission(UMainMission* const Mission)
{
	MainMission = Mission;
}

void UQuestListItem::SetUpQuest(AMasterQuest* const Quest)
{
	AssignedQuest = Quest;
	if(AssignedQuest)
	{
		UpdateQuestListItem();
		SpawnSubGoalList();
	}
}

AMasterQuest* UQuestListItem::GetQuest() const
{
	return AssignedQuest;
}

UBorder* UQuestListItem::GetQuestCategoryBorder() const
{
	return QuestCategoryBorder;
}

UTextBlock* UQuestListItem::GetQuestNameText() const
{
	return QuestNameText;
}

UTextBlock* UQuestListItem::GetLevelNumText() const
{
	return LevelNumText;
}

UTextBlock* UQuestListItem::GetRegionText() const
{
	return RegionText;
}

UButton* UQuestListItem::GetQuestButton() const
{
	return QuestButton;
}

TArray<UMissionSubGoalItem*>& UQuestListItem::GetMissionSubGoalList()
{
	return MissionSubGoalList;
}

#pragma endregion