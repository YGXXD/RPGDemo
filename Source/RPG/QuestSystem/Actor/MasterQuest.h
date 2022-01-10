// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterQuest.generated.h"

UENUM(BlueprintType)//任务类别枚举类
enum class EQuestCategory : uint8
{
	MainlineQuest, //主线任务
	SidelineQuest, //支线任务
	ChallengeQuest //挑战任务
};

UENUM(BlueprintType)//任务类型
enum class ESubGoalType : uint8
{
	Custom, //自定义任务
	Hunt, //打怪任务
	Explore, //探索任务
	Talk //对话任务
};

UENUM(BlueprintType)//任务所在区域
enum class EQuestRegion : uint8
{
	IceLand, //冰岛
	Grotto, //洞穴
	LavaLand //岩浆岛
};

UENUM()
enum class EQuestState : uint8
{
	CanAccept,
	Assigned,
	Complete,
	Failed
};

USTRUCT(BlueprintType)//任务奖励
struct FQuestReward
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	float Money; //金钱

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	float Experience; //经验

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	float Prestige; //声望值
};

USTRUCT(BlueprintType)//任务目的地
struct FSubGoalTargetTransform
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	bool bHasTargetLocation; //是否有目标地点

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FVector TargetLocation; //目标地点

	bool operator==(const FSubGoalTargetTransform& QuestTargetLocation) const;
};

USTRUCT(BlueprintType)//分支任务信息
struct FSubGoalInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	ESubGoalType Type; //任务类型

	// UPROPERTY(EditDefaultsOnly, Category = "Quest")
	// bool bIsCustomSubGoal; //是否为自定义分支任务

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FString CustomSubGoalText; //自定义分支任务文本
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	int TargetID;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FString TargetName; //目标名字

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TSubclassOf<class AActor> TargetActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	int HuntAmount; //击败目标的数量(打怪数量)

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FSubGoalTargetTransform TargetTransform; //任务目标地点

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	bool bCanUpdateQuestDescription; //是否可以更新任务描述

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FString UpdateDescription; //任务更新信息文本

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TArray<int> FollowingSubGoalIDList; //分支任务的索引表
	
	bool operator==(const FSubGoalInfo& SubGoalInfo) const;
};

USTRUCT(BlueprintType)//任务信息
struct FQuestInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FString Name; //任务名字

	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	int SuggestLevel;
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	EQuestCategory Category; //任务类别
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FString Description; //任务描述
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	EQuestRegion Region; //任务所在区域
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	FQuestReward Reward; //任务奖励
	
	UPROPERTY(EditDefaultsOnly, Category = "Quest")
	TArray<FSubGoalInfo> SubGoalInfoList; //分支任务列表
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteCurrentSubGoalListComponentAndUMG, class AMasterQuest*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCompleteSubGoalUpdateUMG, const class AMasterQuest*, Quest);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTargetNumAddUpdateSubGoalItemUMGInfo, const class AMasterQuest*, Quest, int, TargetNumListID);

UCLASS()
class RPG_API AMasterQuest : public AActor
{
	GENERATED_BODY()
private:
	
	UPROPERTY(EditDefaultsOnly,Category="Quest")
	FQuestInfo QuestInfo; //任务信息

	UPROPERTY(EditDefaultsOnly,Category="Quest")
	TArray<int> StartSubGoalIDList; //开始时分支任务索引表

	UPROPERTY()
	TArray<int> CurrentSubGoalTargetNumList; //当前任务中打小怪,或者找东西的数量表

	UPROPERTY()
	TArray<int> CurrentSubGoalInQuestIDList; //当前分支任务索引表,储存的索引为Quest里总SubGoal的索引

	UPROPERTY()
	TArray<FSubGoalInfo> CurrentSubGoalInfoList; //当前分支任务表

	UPROPERTY()
	TArray<FSubGoalInfo> CompleteSubGoalInfoList; //完成的任务表
	
	
	void UpdateSubGoalInfoList();//更新分支任务表信息
	void SetUpStartSubGoalList();//设置开始时分支任务表信息
	bool StartNextSubGoal();//开始下一个任务
	void UpdateFollowingSubGoal();//更新完成任务列表
	void AddTargetNumRecall(int TargetNumListIndex);
	void CompleteCurrentSubGoalReCall();//完成任务回调
	void CompleteSubGoalReCall();//完成分支回调
	
public:	
	AMasterQuest(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	FOnTargetNumAddUpdateSubGoalItemUMGInfo OnTargetNumAddUpdateSubGoalInfo;
	FOnCompleteCurrentSubGoalListComponentAndUMG OnCompleteCurrentSubGoalList;
	FOnCompleteSubGoalUpdateUMG OnCompleteSubGoal;
	
	TArray<int>& GetCurrentSubGoalTargetNumList();
	FQuestInfo& GetQuestInfo();
	TArray<FSubGoalInfo>& GetCurrentSubGoalInfoList();
	TArray<FSubGoalInfo>& GetCompleteSubGoalInfoList();
	TArray<int>& GetCurrentSubGoalIDList();
	//目标数量+1
	void AddTargetNum(int TargetNumListID);
	//完成任务
	void CompleteSubGoal(int SubGoalIndex);
	//静态函数，用于查找SubGoal在SubGoalList中的索引
	int FindSubGoalInfoIndex(const FSubGoalInfo& SubGoalInfo);
};
