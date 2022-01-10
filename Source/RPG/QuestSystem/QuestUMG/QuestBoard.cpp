// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestBoard.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/GameSystem/WidgetPool.h"
#include "RPG/QuestSystem/Actor/MasterQuest.h"
#include "RPG/QuestSystem/QuestUMG/SubGoalBoard.h"

bool UQuestBoard::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	SubGoalBoardIndex = 0;
	
	QuestNameText = Cast<UTextBlock>(GetWidgetFromName("QuestNameText"));
	SubGoalTypeText = Cast<UTextBlock>(GetWidgetFromName("SubGoalType"));
	SubGoalInfoText = Cast<UTextBlock>(GetWidgetFromName("SubGoalInfoText"));;
	SubGoalVerticalBox = Cast<UVerticalBox>(GetWidgetFromName("SubGoalVerticalBox"));
	SubGoalIcon = Cast<UImage>(GetWidgetFromName("SubGoalIcon"));
	UBorder* MainBorder = Cast<UBorder>(GetWidgetFromName("MainBorder"));
	if(MainBorder)
	{
		MainBorder->SetContentColorAndOpacity(FLinearColor(1,1,1,0));
		MainBorder->SetRenderTranslation(FVector2D(-600,0));
	}
	//动态加载Class要路径最后_C
	SubGoalBoardClass =
		LoadClass<USubGoalBoard>(this,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Quest/WBP_SubGoalBoard.WBP_SubGoalBoard_C'"));
	if(SubGoalBoardClass)
	{
		CREATE_WIDGET_POOL(SubGoalBoardClass);
	}
	
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	if(WidgetClass)
	{
		//AnimationMap的KeyValue:”ShowQuestBoard_INST“和"ShowText_INST"
		for (int i = 0; i < WidgetClass->Animations.Num(); i++)
		{
			FString AnimationName;
			WidgetClass->Animations[i]->GetName(AnimationName);
			WidgetAnimationMap.Add(AnimationName,WidgetClass->Animations[i]);
		}
	}
	
	return true;
}

#pragma region Set/Get

AMasterQuest* UQuestBoard::GetAssignedQuest() const
{
	return AssignedQuest;
}

#pragma endregion

#pragma region 更新任务面板信息和面板交互

void UQuestBoard::SetQuestToUpdate(AMasterQuest* const Quest)
{
	if(Quest)
	{
		AssignedQuest = Quest;
		const FQuestInfo QuestInfo = AssignedQuest->GetQuestInfo();
		if(QuestNameText)
		{
			QuestNameText->SetText(FText::FromString(QuestInfo.Name));
		}
		SpawnSubGoalBoardList();
		UpdateCurrentSubGoalBoardList();
		ShowQuestBoard();
	}
}

void UQuestBoard::SpawnSubGoalBoardList()
{
	if(SubGoalVerticalBox)
	{
		CurrentSelectSubGoalBoard = nullptr;
		SubGoalBoardList.Empty();
		WIDGET_POOL->RemoveWidgetAllChildren<USubGoalBoard>(SubGoalBoardClass,SubGoalVerticalBox);
		const TArray<FSubGoalInfo> CurrentSubGoalInfoList = AssignedQuest->GetCurrentSubGoalInfoList();
		for (int i = 0; i < CurrentSubGoalInfoList.Num(); i++)
		{
			if (SubGoalBoardClass)
			{
				USubGoalBoard* SubGoalBoard = WIDGET_POOL->GetUserWidgetFromUMGPool<USubGoalBoard>(SubGoalBoardClass,GetWorld());
				if (SubGoalBoard)
				{
					SubGoalBoard->SetQuestSubGoalID(AssignedQuest->FindSubGoalInfoIndex(CurrentSubGoalInfoList[i]));
					SubGoalBoard->SetQuestBoard(this);
					SubGoalBoard->UpdateSubGoalBoard(AssignedQuest,i);
					SubGoalBoard->UpdateSubGoalState(AssignedQuest);
					SubGoalBoardList.Add(SubGoalBoard);
					SubGoalVerticalBox->AddChildToVerticalBox(SubGoalBoard);
				}
			}
		}
	}
}


void UQuestBoard::UpdateCurrentSubGoalBoardList()
{
	//如果SubGoalBoardList中SubGoalBoard的数量为0，则不更新
	if(!SubGoalBoardList.Num())
	{
		CurrentSelectSubGoalBoard = nullptr;
		SubGoalInfoText->SetText(FText::FromString(""));
		SubGoalTypeText->SetColorAndOpacity(FSlateColor());
		SubGoalTypeText->SetText(FText::FromString(""));
		UMaterialInstance* NoneIconMaterialInstance =
			LoadObject<UMaterialInstance>(this,TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_TargetNone.MI_TargetNone'"));
		SubGoalIcon->SetBrushFromMaterial(NoneIconMaterialInstance);
		return;
	}

	//如果还有任务，更新任务UI面板
	FSubGoalInfo SubGoalInfo;
	if(CurrentSelectSubGoalBoard)
	{
		SubGoalInfo = AssignedQuest->GetQuestInfo().SubGoalInfoList[CurrentSelectSubGoalBoard->GetQuestSubGoalID()];;
	}
	else
	{
		CurrentSelectSubGoalBoard = SubGoalBoardList[0];
		SubGoalInfo = AssignedQuest->GetQuestInfo().SubGoalInfoList[CurrentSelectSubGoalBoard->GetQuestSubGoalID()];;
	}
	
	UMaterialInstance* SubGoalIconMaterialInstance = nullptr;
	FString Type = "";
	FSlateColor TextColor = FSlateColor();
	switch (SubGoalInfo.Type)
	{
	case ESubGoalType::Custom:
		Type = TEXT("事件");
		TextColor = FSlateColor(FLinearColor(1,0.65,0));
		SubGoalIconMaterialInstance = LoadObject<UMaterialInstance>(this,
			TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_TargetOrange.MI_TargetOrange'"));
		break;

	case ESubGoalType::Hunt:
		Type = TEXT("猎杀");
		TextColor = FSlateColor(FLinearColor(1,0,0));
		SubGoalIconMaterialInstance = LoadObject<UMaterialInstance>(this,
			TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_TargetRed.MI_TargetRed'"));
		break;

	case ESubGoalType::Explore:
		Type = TEXT("探索");
		TextColor = FSlateColor(FLinearColor(1,1,0));
		SubGoalIconMaterialInstance = LoadObject<UMaterialInstance>(this,
			TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_TargetYellow.MI_TargetYellow'"));
		break;

	case ESubGoalType::Talk:
		Type = TEXT("交谈");
		TextColor = FSlateColor(FLinearColor(0,0,1));
		SubGoalIconMaterialInstance = LoadObject<UMaterialInstance>(this,
			TEXT("MaterialInstanceConstant'/Game/RPG_Game/Textures/MI_TargetBlue.MI_TargetBlue'"));
		break;
		
	default:
		break;
	}
	SubGoalInfoText->SetText(CurrentSelectSubGoalBoard->GetSubGoalInfoText());
	SubGoalTypeText->SetColorAndOpacity(TextColor);
	SubGoalTypeText->SetText(FText::FromString(Type));
	SubGoalIcon->SetBrushFromMaterial(SubGoalIconMaterialInstance);

	if(CurrentSelectSubGoalBoard)
	{
		FSubGoalInfo CurrentSubGoalInfo = AssignedQuest->GetQuestInfo().SubGoalInfoList[CurrentSelectSubGoalBoard->GetQuestSubGoalID()];
		OnMiniMapTargetShow.Broadcast(CurrentSubGoalInfo.TargetTransform.bHasTargetLocation,CurrentSubGoalInfo.TargetTransform.TargetLocation);
	}
	else
	{
		OnMiniMapTargetShow.Broadcast(false,FVector(0,0,0));
	}
}

void UQuestBoard::SelectSubGoalBoard(USubGoalBoard* SubGoalBoard)
{
	if(CurrentSelectSubGoalBoard)
	{
		CurrentSelectSubGoalBoard->GetSelectButton()->SetIsEnabled(true);
	}
	
	if(SubGoalBoard)
	{
		CurrentSelectSubGoalBoard = SubGoalBoard;
		CurrentSelectSubGoalBoard->GetSelectButton()->SetIsEnabled(false);
	}
	else
	{
		CurrentSelectSubGoalBoard = nullptr;
	}
	UpdateCurrentSubGoalBoardList();
}

void UQuestBoard::UpdateSubGoalBoardListState()
{
	for (int i = 0; i < SubGoalBoardList.Num(); i++)
	{
		SubGoalBoardList[i]->UpdateSubGoalState(AssignedQuest);
	}
}

void UQuestBoard::UpdateQuestBoard()
{
	HideSubGoalList();
	auto UpdateQuestBoardLambda = [this]()
	{
		SpawnSubGoalBoardList();
		UpdateCurrentSubGoalBoardList();
		if(SubGoalBoardList.Num())//如果还有任务
		{
			ShowSubGoalList();
		}
		else//如果没有任务
		{
			HideQuestBoard();
		}
	};
	const FTimerDelegate UpdateQuestBoardDelegate = FTimerDelegate::CreateLambda(UpdateQuestBoardLambda);
	GetWorld()->GetTimerManager().SetTimer(UpdateQuestHandle,UpdateQuestBoardDelegate,1.5,false);
}

void UQuestBoard::UpdateSubGoalBoardItemInfo(int SubGoalID)
{
	if(SubGoalID < SubGoalBoardList.Num() && SubGoalID >= 0)
	{
		SubGoalBoardList[SubGoalID]->UpdateSubGoalBoard(AssignedQuest,SubGoalID);
		if(CurrentSelectSubGoalBoard == SubGoalBoardList[SubGoalID])
		{
			SubGoalInfoText->SetText(CurrentSelectSubGoalBoard->GetSubGoalInfoText());
		}
	}
}

#pragma endregion 

#pragma region 展示面板的动画

void UQuestBoard::ShowQuestBoard()
{
	if(WidgetAnimationMap.Contains("ShowQuestBoard_INST"))
	{
		PlayAnimation(WidgetAnimationMap["ShowQuestBoard_INST"]);
	}

	ShowSubGoalList();
}

void UQuestBoard::ShowSubGoalList()
{
	SubGoalBoardIndex = 0;
	auto ShowSubGoalLambda = [this]()
	{
		if(SubGoalBoardIndex < SubGoalBoardList.Num())
		{
			SubGoalBoardList[SubGoalBoardIndex]->ShowSubGoalBoard();
			SubGoalBoardIndex++;
		}
		else
		{
			if(WidgetAnimationMap.Contains("ShowText_INST"))
			{
				PlayAnimation(WidgetAnimationMap["ShowText_INST"]);
			}
			GetWorld()->GetTimerManager().PauseTimer(ShowSubGoalHandle);
		}
	};
	
	const FTimerDelegate ShowSubGoalDelegate = FTimerDelegate::CreateLambda(ShowSubGoalLambda);
	GetWorld()->GetTimerManager().SetTimer(ShowSubGoalHandle,ShowSubGoalDelegate,0.2,true);
}

void UQuestBoard::HideQuestBoard()
{
	if(WidgetAnimationMap.Contains("ShowQuestBoard_INST"))
	{
		PlayAnimation(WidgetAnimationMap["ShowQuestBoard_INST"],0,1,EUMGSequencePlayMode::Reverse,1);
	}
}

void UQuestBoard::HideSubGoalList()
{
	SubGoalBoardIndex = 0;
	auto HideSubGoalLambda = [this]()
	{
		if(SubGoalBoardIndex < SubGoalBoardList.Num())
		{
			SubGoalBoardList[SubGoalBoardIndex]->HideSubGoalBoard();
			SubGoalBoardIndex++;
		}
		else
		{
			GetWorld()->GetTimerManager().PauseTimer(HideSubGoalHandle);
		}
	};
	
	const FTimerDelegate HideSubGoalDelegate = FTimerDelegate::CreateLambda(HideSubGoalLambda);
	GetWorld()->GetTimerManager().SetTimer(HideSubGoalHandle,HideSubGoalDelegate,0.1,true);
}

#pragma endregion