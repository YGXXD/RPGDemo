// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "MissionSubGoalItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMissionSubGoalItem : public UUserWidget
{
	GENERATED_BODY()
private:
	int QuestSubGoalID;
	
	UPROPERTY()
	class UImage* SubGoalStateImage;

	UPROPERTY()
	class UTextBlock* SubGoalText;

	virtual bool Initialize() override;
public:
	void UpdateMissionSubGoalItem(class AMasterQuest* AssignedQuest,int CurrentSubGoalID);
	void UpdateSubGoalItemState(class AMasterQuest* AssignedQuest);
	void SetQuestSubGoalID(int ID);
};
