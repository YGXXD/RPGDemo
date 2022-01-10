// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "QuestComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UQuestComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	bool bHasSubGoalTargetLocation;
	FVector CurrentTargetLocation;
	
	UPROPERTY()
	TArray<TSubclassOf<class AMasterQuest>> AllQuestClassList;

	UPROPERTY()
	TArray<class AMasterQuest*> AssignedQuestList;

	UPROPERTY()
	TArray<class AMasterQuest*> CompleteQuestList;

	UPROPERTY()
	TArray<class AMasterQuest*> FailedQuestList;

	UPROPERTY()
	TArray<class AMasterQuest*> CanAcceptQuestList;

	UPROPERTY()
	class ARPGameCharacter* Player;
	
	UPROPERTY()
	class UPlayerControllerMainUI* PCMainUI;

	//MiniMap上更新方向和距离
	float UpdateTargetDistance();
	void UpdateTargetDirection();
	UFUNCTION()
	void MiniMapTargetShow(bool bHasTargetLocation, FVector TargetLocation);

public:	
	UQuestComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void SetPlayerAndMainUI(class ARPGameCharacter* const RPGameCharacter,class UPlayerControllerMainUI* const MainUI);
	//在移动时更新任务目标方向与距离
	void UpdateTargetOnMove();
	//添加任务
	void AddQuest(TSubclassOf<class AMasterQuest> QuestClass);
	//任务目标数量+1
	void OnSubGoalTargetComplete(TSubclassOf<class AActor> TargetClass);
	//完成任务
	bool TryCompleteQuest(TSubclassOf<class AMasterQuest> QuestClass);

	//保存任务
	void SaveQuest() const;
	void LoadQuest(int Index);
	
	/////////////
	//完成SubGoal,测试用
	void CompleteSubGoal(int SubGoalIndex);
};
