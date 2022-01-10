// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuestBoard.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMiniMapTargetShowDelegate, bool, bHasTargetLocation, FVector, TargetLocation);
UCLASS()
class RPG_API UQuestBoard : public UUserWidget
{
	GENERATED_BODY()
private:
	int SubGoalBoardIndex;//用来遍历SubGoalBoardList的变量
	FTimerHandle ShowSubGoalHandle;
	FTimerHandle HideSubGoalHandle;
	FTimerHandle UpdateQuestHandle;

	UPROPERTY()
	TSubclassOf<class USubGoalBoard> SubGoalBoardClass;
	
	UPROPERTY()
	TArray<class USubGoalBoard*> SubGoalBoardList;

	UPROPERTY()
	class USubGoalBoard* CurrentSelectSubGoalBoard;
	
	UPROPERTY()
	class AMasterQuest* AssignedQuest;
	
	UPROPERTY()
	class UTextBlock* QuestNameText;

	UPROPERTY()
	class UTextBlock* SubGoalTypeText;

	UPROPERTY()
	class UTextBlock* SubGoalInfoText;
	
	UPROPERTY()
	class UVerticalBox* SubGoalVerticalBox;

	UPROPERTY()
	class UImage* SubGoalIcon;

	UPROPERTY()
	TMap<FString,class UWidgetAnimation*> WidgetAnimationMap;

	virtual bool Initialize() override;
	void SpawnSubGoalBoardList();
	void UpdateCurrentSubGoalBoardList();
	
	//SubGoalBoardList的动画
	void ShowSubGoalList();
	void HideSubGoalList();

public:
	//展示MiniMap的TargetDistance和Direction的委托
	FOnMiniMapTargetShowDelegate OnMiniMapTargetShow;
	//QuestBoard的动画
	void ShowQuestBoard();
	void HideQuestBoard();
	
	void UpdateSubGoalBoardListState();//更新SubGoal完成状态
	void SetQuestToUpdate(class AMasterQuest* const Quest);//在设置任务时更新任务
	void UpdateQuestBoard();
	void SelectSubGoalBoard(class USubGoalBoard* SubGoalBoard);
	void UpdateSubGoalBoardItemInfo(int SubGoalID);

	AMasterQuest* GetAssignedQuest() const;
};
