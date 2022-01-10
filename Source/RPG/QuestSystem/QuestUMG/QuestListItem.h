// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestListItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UQuestListItem : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TSubclassOf<class UMissionSubGoalItem> MissionSubGoalItemClass;
	
	UPROPERTY()
	class UBorder* QuestCategoryBorder;
	
	UPROPERTY()
	class UBorder* ItemFrame;
	
	UPROPERTY()
	class UTextBlock* QuestNameText;
	
	UPROPERTY()
	class UTextBlock* LevelNumText;

	UPROPERTY()
	class UTextBlock* RegionText;

	UPROPERTY()
	class UButton* QuestButton;

	UPROPERTY()
	class UMainMission* MainMission;

	UPROPERTY()
	class AMasterQuest* AssignedQuest;

	UPROPERTY()
	TArray<class UMissionSubGoalItem*> MissionSubGoalList;

	virtual bool Initialize() override;
	
	UFUNCTION()
	void OnQuestButtonClick();


public:
	//接任务的角色等级不同,建议等级呈现出不同的颜色
	//MainMission调用
	void UpdateLevelColor(int Level);
	void UpdateQuestListItem();
	void SpawnSubGoalList();
	void UpdateMissionSubGoalListState();
	void UpdateMissionSubGoalItemInfo(int MissionSubGoalItemID);
	
	void SetUpQuest(class AMasterQuest* const Quest);
	void SetMainMission(class UMainMission* const Mission);
	AMasterQuest* GetQuest() const;
	UBorder* GetQuestCategoryBorder() const;
	UTextBlock* GetQuestNameText() const;
	UTextBlock* GetLevelNumText() const;
	UTextBlock* GetRegionText() const;
	UButton* GetQuestButton() const;
	TArray<class UMissionSubGoalItem*>& GetMissionSubGoalList();
};
