// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMission.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMainMission : public UUserWidget
{
	GENERATED_BODY()
private:
	FTimerHandle QuestListDelayHandle;//显示QuestList的动画的延时计时器
	
	UPROPERTY()
	class ARPGameCharacter* Player;
	
	UPROPERTY()
	TMap<FString,class UWidgetAnimation*> WidgetAnimationMap;

	UPROPERTY()
	class UQuestBoard* QuestBoard;

	UPROPERTY()
	TSubclassOf<class UQuestListItem> QuestListItemClass;
	
	UPROPERTY()
	TMap<class AMasterQuest*,class UQuestListItem*> AssignedQuestItemMap;
	
#pragma region UMG控件
	
	//任务列表切换按钮
	UPROPERTY()
	class UButton* AssignedQuestButton;

	UPROPERTY()
	class UButton* CompleteQuestButton;

	UPROPERTY()
	class UButton* FailedQuestButton;

	UPROPERTY()
	class UButton* CanAcceptQuestButton;

	//任务列表滚动框
	UPROPERTY()
	class UScrollBox* AssignedQuestScrollBox;

	UPROPERTY()//已接受任务列表
	TArray<class UQuestListItem*> AssignedQuestList;

	UPROPERTY()//当前选中的已接受任务
	class UQuestListItem* SelectedAssignedQuest;

	UPROPERTY()
	class UScrollBox* CompleteQuestScrollBox;

	UPROPERTY()//完成任务列表
	TArray<class UQuestListItem*> CompleteQuestList;

	UPROPERTY()//当前选中的完成任务
	class UQuestListItem* SelectedCompleteQuest;

	UPROPERTY()
	class UScrollBox* FailedQuestScrollBox;

	UPROPERTY()//失败任务列表
	TArray<class UQuestListItem*> FailedQuestList;

	UPROPERTY()//当前选中的失败任务
	class UQuestListItem* SelectedFailedQuest;

	UPROPERTY()
	class UScrollBox* CanAcceptQuestScrollBox;

	UPROPERTY()//可接受任务列表
	TArray<class UQuestListItem*> CanAcceptQuestList;

	UPROPERTY()//当前选中的可接受任务
	class UQuestListItem* SelectedCanAcceptQuest;

	UPROPERTY()//当前选中的的滚动框
	class UScrollBox* CurrentSelectedScrollBox;

	//Description的控件
	UPROPERTY()
	class UQuestListItem* CurrentSelectQuestListItem;
	
	UPROPERTY()
	class UScrollBox* QuestDetailScrollBox;
	
	UPROPERTY()
	class UTextBlock* DescriptionText;

	UPROPERTY()
	class UTextBlock* QuestNameText;

	UPROPERTY()
	class UImage* QuestCategoryImage;

	UPROPERTY()
	class UTextBlock* QuestCategoryText;
	
	UPROPERTY()
	class UTextBlock* RegionText;

	UPROPERTY()
	class UTextBlock* SuggestLevelText;

	//Description中的Reward奖励控件
	UPROPERTY()
	class UVerticalBox* RewardBox;
	
	UPROPERTY()
	class UHorizontalBox* MoneyHorizontalBox;

	UPROPERTY()
	class UTextBlock* MoneyRewardText;

	UPROPERTY()
	class UHorizontalBox* ExpHorizontalBox;

	UPROPERTY()
	class UTextBlock* ExpRewardText;

	UPROPERTY()
	class UHorizontalBox* PrestigeHorizontalBox;

	UPROPERTY()
	class UTextBlock* PrestigeRewardText;

	//Description中的任务目标
	UPROPERTY()
	class UVerticalBox* SubGoalInfoBox;
	
	UPROPERTY()
	class UVerticalBox* SubGoalVerticalBox;

	//Description中的选择与取消按钮
	UPROPERTY()
	class UButton* SelectQuestButton;

	UPROPERTY()
	class UButton* CancelQuestButton;

#pragma endregion 

	virtual bool Initialize() override;
	
	//选择任务列表Button函数绑定
	UFUNCTION()
	void OnAssignedQuestButtonClick();

	UFUNCTION()
	void OnCompleteQuestButtonClick();

	UFUNCTION()
	void OnFailedQuestButtonClick();

	UFUNCTION()
	void OnCanAcceptQuestButtonClick();

	//Description中的选择与取消按钮函数绑定
	UFUNCTION()
	void OnSelectQuestButtonClick();

	UFUNCTION()
	void OnCancelQuestButtonClick();
	
	UFUNCTION()//绑定角色升级更新任务委托
	void UpdateQuestListSuggestLevel(int Level);
	//动画
	float AddQuestList();//返回动画时长
	float RemoveQuestList();//返回动画时长
	//更新任务列表
	void UpdateAssignedQuestScrollBox();
	void UpdateCompleteQuestScrollBox();
	void UpdateFailedQuestScrollBox();
	void UpdateCanAcceptScrollBox();
	//更新任务细节
	void UpdateQuestDetail();
	void UpdateSubGoalInfoBox();
public:
	void SetPlayer(class ARPGameCharacter* const RPGameCharacter);
	void SetPlayerQuestBoard(class UQuestBoard* const PlayerQuestBoard);
	void UpdateAssignedQuest(TArray<class AMasterQuest*>& QuestList);
	void UpdateCompleteQuest(TArray<class AMasterQuest*>& QuestList);
	UQuestListItem* GetQuestListItem(const class AMasterQuest* Quest);
	//选择任务
	void SelectQuestListItem(class UQuestListItem* QuestListItem);
	
	UFUNCTION()//绑定了任务的委托OnCompleteSubGoal会自动更新
	void UpdateAllSubGoalStateInQuest(const class AMasterQuest* Quest);
	UFUNCTION()//绑定了任务委托OnCompleteCurrentSubGoalList
	void UpdateQuestFollowingSubGoal(class AMasterQuest* Quest);
	//更新当前的任务列表框，在完成任务后
	void UpdateCurrentSelectedScrollBox();

	//更新单项SubGoal
	UFUNCTION()//绑定任务SubGoal更新委托
	void UpdateSubGoalItemInfo(const class AMasterQuest* Quest, int CurrenSubGoalID);
};
