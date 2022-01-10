// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Character.h"
#include "RPG/Player/Interface/InteractionInterface.h"
#include "RPG/Player/Interface/SaveLoadInterface.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "NPCharacterBase.generated.h"

UENUM(BlueprintType)
enum class ETalkType : uint8
{
	ElseInfo,//其他信息
	QuestInfo,//任务信息
	EquipStore,//装备商店
	DrugStore,//药店
	FoodStore,//食品店
	End //结束对话
};

UENUM(BlueprintType)
enum class ESubTalkType : uint8
{
	None,
	Continue,
	Back,
	AcceptQuest,
	Quit
};

USTRUCT(BlueprintType)
struct FSubTalkInfo
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FString SubTalkInfo;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	ESubTalkType SelectSubTalkType;
	
};

USTRUCT()
struct FTalkContent //说话内容结构体
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FString TalkerName;//说话人姓名
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FString TalkInfo;//说话内容
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	bool bHasSubTalkSelect;//是否有说话分支
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	TArray<FSubTalkInfo> SubTalkInfoList;//说话分支的信息
};

USTRUCT()
struct FDialogueContent : public FTableRowBase//对话交谈结构体
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FString SelectTalkInfoString;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	TArray<FTalkContent> TalkContentList;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	ETalkType TalkType;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	int CanAcceptQuestLevel;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	TSubclassOf<class AMasterQuest> QuestClass;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FString AcceptQuestSelectTalkInfoString;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FTalkContent AcceptQuestTalkContent;

	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FTalkContent FinishQuestTalkContent;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	EQuestState QuestState;
	
};

UCLASS()
class RPG_API ANPCharacterBase : public ACharacter, public IInteractionInterface, public ISaveLoadInterface
{
	GENERATED_BODY()

public:
	ANPCharacterBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	bool bIsNormalNPC;
	
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	FString Name;

	//NPC随机的对话
	UPROPERTY(EditDefaultsOnly,Category="Talk")
	TArray<FText> RandomTalkInfoList;

	UPROPERTY(EditDefaultsOnly,Category="Talk")
	class UDataTable* DialogueDataTable;
	
	UPROPERTY()
	TArray<FDialogueContent> DialogueList;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* WidgetComp;
	
	//小地图图标组件
	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* PaperSpriteComp;

	//玩家的指针
	UPROPERTY()
	class ARPGameCharacter* Player;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void InitializeCharacter();
	
	UFUNCTION()
	void UpdateQuestInformation(int PlayerLevel); //更新NPC小地图上任务标志
	UFUNCTION()
	virtual void InteractWithTargetObject() override;
	void NormalTalk();//普通说话，改变UI
	void InteractTalk();//和玩家交互对话
	
public:
	void AcceptQuestFromNPC(int TalkIndex);
	bool CompleteQuestFromNPC(int TalkIndex);
	TArray<FDialogueContent>& GetDialogueList();
	//交互接口实现
	virtual void BeginOverlapTargetObject() override;
	virtual void EndOverlapTargetObject() override;

	virtual void SaveInfo() override;
	virtual void LoadInfo(int Index) override;
};
