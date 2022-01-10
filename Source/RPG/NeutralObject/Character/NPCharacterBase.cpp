// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/DataTableFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/NeutralObject/InteractionUMG/NeutralObjectInteraction.h"
#include "RPG/NeutralObject/InteractionUMG/SelectTalkBox.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/Player/ControllerUMG/PlayerControllerMainUI.h"
#include "RPG/QuestSystem/Component/QuestComponent.h"
#include "RPG/LoadSystem/SaveData/RPGSaveGame.h"

// Sets default values
ANPCharacterBase::ANPCharacterBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	Name = "NPC";
	bIsNormalNPC = false;
    
	//初始化UMG组件
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(GetCapsuleComponent());
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UNeutralObjectInteraction> WBP_NeutralObjectInteraction(TEXT("/Game/RPG_Game/UI/Talk/WBP_NeutralObjectInteraction"));
	if(WBP_NeutralObjectInteraction.Succeeded())
	{
		WidgetComp->SetWidgetClass(WBP_NeutralObjectInteraction.Class);
	}
	WidgetComp->SetHiddenInGame(true);
	
	//初始化PaperSprite组件
	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	PaperSpriteComp->SetupAttachment(GetCapsuleComponent());
	static ConstructorHelpers::FObjectFinder<UPaperSprite> MiniMapCharacterSprite(TEXT("PaperSprite'/Game/RPG_Game/Textures/MiniMapNPC_Sprite.MiniMapNPC_Sprite'"));
	if(MiniMapCharacterSprite.Succeeded())
	{
		PaperSpriteComp->SetSprite(MiniMapCharacterSprite.Object);
	}
	PaperSpriteComp->SetSpriteColor(FLinearColor(1,1,0));
	PaperSpriteComp->SetRelativeLocation(FVector(0,0,140));
	PaperSpriteComp->SetRelativeRotation(FRotator(0,90,-90));
	PaperSpriteComp->SetRelativeScale3D(FVector(0.5,0.5,0.5));
	PaperSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PaperSpriteComp->SetOwnerNoSee(true);
	
}

void ANPCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	InitializeCharacter();
}

void ANPCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FDialogueContent>& ANPCharacterBase::GetDialogueList()
{
	return DialogueList;
}

void ANPCharacterBase::InitializeCharacter()
{
	if(DialogueDataTable)
	{
		bIsNormalNPC = false;
		TArray<FName> RowNameList;
		UDataTableFunctionLibrary::GetDataTableRowNames(DialogueDataTable,RowNameList);
		for (int i = 0; i < RowNameList.Num(); i++)
		{
			FString ContextString;
			FDialogueContent OutRow;
			FDialogueContent DialogueContent = *DialogueDataTable->FindRow<FDialogueContent>(RowNameList[i],ContextString);
			DialogueList.Add(DialogueContent);
		}
	}
	else
	{
		bIsNormalNPC = true;
	}
	
	Player = Cast<ARPGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if(Player)
	{
		SetOwner(Player);
		//绑定等级更新后更新任务委托
		Player->OnLevelUpUpdateQuest.AddDynamic(this,&ANPCharacterBase::UpdateQuestInformation);
	}
	UNeutralObjectInteraction* NeutralObjectWidget = Cast<UNeutralObjectInteraction>(WidgetComp->GetUserWidgetObject());
	NeutralObjectWidget->SetNameText(Name);
}

#pragma region 交互接口实现

void ANPCharacterBase::BeginOverlapTargetObject()
{
	WidgetComp->SetHiddenInGame(false);
	Player->OnInteract.AddDynamic(this,&ANPCharacterBase::InteractWithTargetObject);
}

void ANPCharacterBase::EndOverlapTargetObject()
{
	WidgetComp->SetHiddenInGame(true);
	Player->OnInteract.RemoveDynamic(this,&ANPCharacterBase::InteractWithTargetObject);
}

void ANPCharacterBase::InteractWithTargetObject()
{
	if(bIsNormalNPC)
	{
		NormalTalk();
	}
	else
	{
		InteractTalk();
	}
	Player->GetQuestComponent()->OnSubGoalTargetComplete(GetClass());
}

#pragma endregion 

#pragma region 对话和任务更新

void ANPCharacterBase::NormalTalk()
{
	const int RandomIndex = FMath::RandHelper(RandomTalkInfoList.Num());
	if(RandomTalkInfoList.Num())
	{
		const FText RandomInfo = RandomTalkInfoList[RandomIndex];
		UNeutralObjectInteraction* NeutralObjectWidget = Cast<UNeutralObjectInteraction>(WidgetComp->GetUserWidgetObject());
		NeutralObjectWidget->ShowTalkInfo(RandomInfo);
	}
}

void ANPCharacterBase::InteractTalk()
{	
	USelectTalkBox* SelectTalkBox = Player->GetMainUI()->GetSelectTalkBox();
	SelectTalkBox->SetUpCharacterToUpdate(this);
}

void ANPCharacterBase::UpdateQuestInformation(int PlayerLevel)
{
	for (int i = 0; i < DialogueList.Num(); i++)
	{
		if(DialogueList[i].TalkType == ETalkType::QuestInfo && DialogueList[i].QuestState == EQuestState::CanAccept && DialogueList[i].CanAcceptQuestLevel <= PlayerLevel)
		{
			UPaperSprite* MiniMapQuest = LoadObject<UPaperSprite>(this,TEXT("PaperSprite'/Game/RPG_Game/Textures/MiniMapQuest_Sprite.MiniMapQuest_Sprite'"));
			if(MiniMapQuest)
			{
				PaperSpriteComp->SetSpriteColor(FLinearColor(1,0,0));
				PaperSpriteComp->SetSprite(MiniMapQuest);
			}
			return;
		}
	}

	UPaperSprite* MiniMapCharacterSprite = LoadObject<UPaperSprite>(this,TEXT("PaperSprite'/Game/RPG_Game/Textures/MiniMapNPC_Sprite.MiniMapNPC_Sprite'"));
	if(MiniMapCharacterSprite)
	{
		PaperSpriteComp->SetSpriteColor(FLinearColor(1,1,0));
		PaperSpriteComp->SetSprite(MiniMapCharacterSprite);
	}
}

void ANPCharacterBase::AcceptQuestFromNPC(int TalkIndex)
{
	DialogueList[TalkIndex].QuestState = EQuestState::Assigned;
	Player->AcceptQuest(DialogueList[TalkIndex].QuestClass);
	UpdateQuestInformation(Player->GetCurrentLevel());
}

bool ANPCharacterBase::CompleteQuestFromNPC(int TalkIndex)
{
	//完成任务的话会返回true，否则返回false
	if(Player->GetQuestComponent()->TryCompleteQuest(DialogueList[TalkIndex].QuestClass))
	{
		DialogueList[TalkIndex].QuestState = EQuestState::Complete;
		return true;
	}
	return false;
}

#pragma endregion

#pragma region 游戏载入

void ANPCharacterBase::SaveInfo()
{
	
}

void ANPCharacterBase::LoadInfo(int Index)
{
	const FSaveQuestList SaveQuestList = SAVE_DATA->GetSavedQuestList(Index);
	for (int i = 0; i < DialogueList.Num(); i++)
	{
		if(SaveQuestList.CompleteQuestList.Contains(DialogueList[i].QuestClass))
		{
			DialogueList[i].QuestState = EQuestState::Complete;
		}
		else if(SaveQuestList.AssignedQuestList.Contains(DialogueList[i].QuestClass))
		{
			DialogueList[i].QuestState = EQuestState::Assigned;
		}
	}
}

#pragma endregion