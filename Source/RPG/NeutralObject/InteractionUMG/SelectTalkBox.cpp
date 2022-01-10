// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectTalkBox.h"

#include "SelectTalkInfo.h"
#include "TalkBoard.h"
#include "Components/ScrollBox.h"
#include "RPG/GameSystem/WidgetPool.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/QuestSystem/Component/QuestComponent.h"

bool USelectTalkBox::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	SelectTalkInfoScrollBox = Cast<UScrollBox>(GetWidgetFromName("SelectTalkInfoScrollBox"));

	SelectTalkInfoClass =
		LoadClass<USelectTalkInfo>(this,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Talk/WBP_SelectTalkInfo.WBP_SelectTalkInfo_C'"));
	if(SelectTalkInfoClass)
	{
		CREATE_WIDGET_POOL(SelectTalkInfoClass);
	}
	return true;
}

void USelectTalkBox::SetTalkBoard(UTalkBoard* const Board)
{
	TalkBoard = Board;
}

void USelectTalkBox::SetPlayer(ARPGameCharacter* const Character)
{
	Player = Character;
}

void USelectTalkBox::SetUpCharacterToUpdate(ANPCharacterBase* const Character)
{
	NPCharacter = Character;
	if(NPCharacter)
	{
		UpdateSelectTalkBox();
	}
}

void USelectTalkBox::UpdateSelectTalkBox()
{
	TArray<FDialogueContent> DialogueList = NPCharacter->GetDialogueList();
	if(SelectTalkInfoScrollBox)
	{
		for (int i = 0; i < DialogueList.Num(); i++)
		{
			if(DialogueList[i].TalkType == ETalkType::QuestInfo)
			{
				if(!DialogueList[i].QuestClass || DialogueList[i].QuestState == EQuestState::Complete || DialogueList[i].CanAcceptQuestLevel > Player->GetCurrentLevel())
				{
					continue;
				}
			}
			if (SelectTalkInfoClass)
			{
				USelectTalkInfo* SelectTalkInfo = WIDGET_POOL->GetUserWidgetFromUMGPool<USelectTalkInfo>(SelectTalkInfoClass,GetWorld());
				if(SelectTalkInfo)
				{
					SelectTalkInfo->SetTalkIndex(i);
					SelectTalkInfo->SetSubTalkType(ESubTalkType::None);
					if(DialogueList[i].QuestState == EQuestState::Assigned)
					{
						SelectTalkInfo->SetSelectTalkInfoString(DialogueList[i].AcceptQuestSelectTalkInfoString);
					}
					else
					{
						SelectTalkInfo->SetSelectTalkInfoString(DialogueList[i].SelectTalkInfoString);
					}
					SelectTalkInfo->SetSelectTalkBox(this);
					SelectTalkInfo->UpdateSelectTalkInfo();
					SelectTalkInfoScrollBox->AddChild(SelectTalkInfo);
				}
			}
		}
	}
}

void USelectTalkBox::AutoPlayDialogue()
{
	auto PlayTalkContentLambda = [this]()
	{
		if (DialogueTalkContentID < CurrentDialogue.TalkContentList.Num())
		{
			const FTalkContent TalkContent = CurrentDialogue.TalkContentList[DialogueTalkContentID];
			if (TalkBoard)
			{
				TalkBoard->ShowTalkInfo(TalkContent.TalkerName, TalkContent.TalkInfo);
				DialogueTalkContentID++;
			}
			if (TalkContent.bHasSubTalkSelect)
			{
				for (int i = 0; i < TalkContent.SubTalkInfoList.Num(); i++)
				{
					if (SelectTalkInfoClass)
					{
						USelectTalkInfo* SelectTalkInfo = WIDGET_POOL->GetUserWidgetFromUMGPool<USelectTalkInfo>(SelectTalkInfoClass, GetWorld());
						if (SelectTalkInfo)
						{
							SelectTalkInfo->SetSubTalkType(TalkContent.SubTalkInfoList[i].SelectSubTalkType);
							SelectTalkInfo->SetSelectTalkInfoString(TalkContent.SubTalkInfoList[i].SubTalkInfo);
							SelectTalkInfo->SetSelectTalkBox(this);
							SelectTalkInfo->UpdateSelectTalkInfo();
							SelectTalkInfoScrollBox->AddChild(SelectTalkInfo);
						}
					}
				}
				GetWorld()->GetTimerManager().PauseTimer(PlayDialogueHandle);
			}
		}
		else
		{
			if (TalkBoard)
			{
				TalkBoard->HideTalkInfo();
			}
			GetWorld()->GetTimerManager().PauseTimer(PlayDialogueHandle);
		}
	};
	const FTimerDelegate PlayTalkContentDelegate = FTimerDelegate::CreateLambda(PlayTalkContentLambda);
	GetWorld()->GetTimerManager().SetTimer(PlayDialogueHandle, PlayTalkContentDelegate, 3, true,0);
}

void USelectTalkBox::SelectTalkInfo(int Index)
{
	CurrentDialogueIndex = Index;
	CurrentDialogue = NPCharacter->GetDialogueList()[Index];
	DialogueTalkContentID = 0;
	switch (CurrentDialogue.TalkType)
	{
	case ETalkType::ElseInfo:
		SelectElseInfo();
		break;

	case ETalkType::QuestInfo:
		SelectQuestInfo();
		break;

	case ETalkType::EquipStore:
		SelectEquipStore();
		break;

	case ETalkType::DrugStore:
		SelectDrugStore();
		break;

	case ETalkType::FoodStore:
		SelectFoodStore();
		break;

	case ETalkType::End:
		SelectEnd();
		break;
		
	default:
		break;
	}
}

void USelectTalkBox::SelectElseInfo()
{
	 WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
}

void USelectTalkBox::SelectQuestInfo()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
	if(CurrentDialogue.QuestState == EQuestState::Assigned)
	{
		FTalkContent TalkContent;
		//完成任务的话会返回true，否则返回false
		if(NPCharacter->CompleteQuestFromNPC(CurrentDialogueIndex))
		{
			TalkContent = CurrentDialogue.FinishQuestTalkContent;
		}
		else
		{
			TalkContent = CurrentDialogue.AcceptQuestTalkContent;
		}
		if(TalkBoard)
		{
			TalkBoard->ShowTalkInfo(TalkContent.TalkerName,TalkContent.TalkInfo);
		}
		auto HideTalkBoardLambda = [this]()
		{
			if(TalkBoard)
			{
				TalkBoard->HideTalkInfo();
			}
		};
		const FTimerDelegate HideTalkBoardDelegate = FTimerDelegate::CreateLambda(HideTalkBoardLambda);
		GetWorld()->GetTimerManager().SetTimer(PlayDialogueHandle,HideTalkBoardDelegate,3,false);
	}
	else
	{
		AutoPlayDialogue();
	}
}

void USelectTalkBox::SelectEquipStore()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
}

void USelectTalkBox::SelectDrugStore()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
}

void USelectTalkBox::SelectFoodStore()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
}

void USelectTalkBox::SelectEnd()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
	if(TalkBoard)
	{
		TalkBoard->HideTalkInfo();
	}
}

void USelectTalkBox::TalkContinue()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
	AutoPlayDialogue();
}

void USelectTalkBox::TalkBack()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
	UpdateSelectTalkBox();
}

void USelectTalkBox::TalkAcceptQuest()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
	NPCharacter->AcceptQuestFromNPC(CurrentDialogueIndex);
	if(TalkBoard)
	{
		TalkBoard->HideTalkInfo();
	}
}

void USelectTalkBox::TalkQuit()
{
	WIDGET_POOL->RemoveWidgetAllChildren<USelectTalkInfo>(SelectTalkInfoClass,SelectTalkInfoScrollBox);
	if(TalkBoard)
	{
		TalkBoard->HideTalkInfo();
	}
}
