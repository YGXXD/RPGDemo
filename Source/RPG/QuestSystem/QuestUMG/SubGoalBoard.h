// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "SubGoalBoard.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API USubGoalBoard : public UUserWidget
{
	GENERATED_BODY()
private:
	int QuestSubGoalID;
	
	UPROPERTY()
	class UQuestBoard* QuestBoard;

	//SubGoalBoard的UMG控件
	UPROPERTY()
	class UBorder* MainBorder;
	
	UPROPERTY()
	class UTextBlock* SubGoalText;
	
	UPROPERTY()
	class UImage* DoneImage;

	UPROPERTY()
	class UButton* SelectButton;

	virtual bool Initialize() override;
	
	UFUNCTION()
	void OnSelectButtonOnclick();

	UPROPERTY()
	class UWidgetAnimation* ShowSubGoalBoardAnimation;
	
public:
	void SetQuestBoard(class UQuestBoard* const Board);
	void SetQuestSubGoalID(int ID);
	int GetQuestSubGoalID() const;
	FText GetSubGoalInfoText() const;
	UButton* GetSelectButton() const;
	
	//更新SubGoal信息
	void UpdateSubGoalBoard(class AMasterQuest* AssignedQuest,int CurrentSubGoalID);
	void UpdateSubGoalState(AMasterQuest* AssignedQuest);
	//动画展示和隐藏
	void ShowSubGoalBoard();
	void HideSubGoalBoard();
	
};
