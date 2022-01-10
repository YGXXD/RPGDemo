// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "RPGSaveGame.generated.h"

/**
 * 
 */

USTRUCT()
struct FCharacterAttribute
{
	GENERATED_BODY()

	UPROPERTY()
	float CurrentValue;
	UPROPERTY()
	float MinValue;
	UPROPERTY()
	float MaxValue;
};

USTRUCT()
struct FSaveAbilityList
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<TSubclassOf<class AAbilityBase>> PlayerAbilityList;
};

USTRUCT()
struct FAssignedQuestInfo
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<int> CurrentSubGoalTargetNumList; //当前任务中打小怪,或者找东西的数量表

	UPROPERTY()
	TArray<int> CurrentSubGoalInQuestIDList; //当前分支任务索引表,储存的索引为Quest里总SubGoal的索引

	UPROPERTY()
	TArray<FSubGoalInfo> CurrentSubGoalInfoList; //当前分支任务表

	UPROPERTY()
	TArray<FSubGoalInfo> CompleteSubGoalInfoList; //完成的任务表
};

USTRUCT()
struct FSaveQuestList
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<TSubclassOf<class AMasterQuest>> AssignedQuestList;

	UPROPERTY()
	TArray<FAssignedQuestInfo> AssignedQuestInfoList;
	
	UPROPERTY()
	TArray<TSubclassOf<class AMasterQuest>> CompleteQuestList;
};

UCLASS()
class RPG_API URPGSaveGame : public USaveGame
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TArray<FString> TimeStringArray;
	
	UPROPERTY()
	TArray<FCharacterAttribute> HealthArray;

	UPROPERTY()
	TArray<FCharacterAttribute> ManaArray;

	UPROPERTY()
	TArray<FCharacterAttribute> StaminaArray;

	UPROPERTY()
	TArray<FCharacterAttribute> ExperienceArray;

	UPROPERTY()
	TArray<FCharacterAttribute> LevelArray;

	UPROPERTY()
	TArray<FCharacterAttribute> MoneyArray;

	UPROPERTY()
	TArray<FTransform> PlayerTransformArray;

	UPROPERTY()
	TArray<FSaveAbilityList> PlayerAbilityListArray;

	UPROPERTY()
	TArray<FSaveQuestList> QuestListArray;


public:
	//删除一个存档
	void DeleteDataByIndex(int Index);
	
	//保存存档时间
	void SaveTimeString(const FString Time);
	TArray<FString> GetTimeArray() const;
	
	//保存角色位置
	void SavePlayerTransform(const FTransform& Transform);
	FTransform GetPlayerTransform(int Index) const;

	//保存角色当前的属性
	void SavePlayerAttribute(const TMap<EAttributeType,FAttribute>& AttributeMap);
	void LoadPlayerAttribute(TMap<EAttributeType,FAttribute>& AttributeMap, int Index) const;

	//保存角色学过的技能类
	void SavePlayerAbility(const TArray<TSubclassOf<class AAbilityBase>>& PlayerAbilityList);
	TArray<TSubclassOf<AAbilityBase>> GetPlayerAbilityList(int Index) const;

	//保存角色的任务类
	void SavePlayerQuest(const TArray<class AMasterQuest*>& AssignedQuestList,const TArray<class AMasterQuest*>& CompleteQuestList);
	FSaveQuestList GetSavedQuestList(int Index) const;
};
