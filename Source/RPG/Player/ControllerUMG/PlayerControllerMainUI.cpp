// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerMainUI.h"

#include "CharacterBorder.h"
#include "GlassBar.h"
#include "LevelBar.h"
#include "MiniMap.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/AbilitySystem/AbilityUMG/SkillBar.h"
#include "RPG/LoadSystem/LoadUMG/GameMenu.h"
#include "RPG/LoadSystem/LoadUMG/GameOverMenu.h"
#include "RPG/NeutralObject/InteractionUMG/SelectTalkBox.h"
#include "RPG/NeutralObject/InteractionUMG/TalkBoard.h"
#include "RPG/QuestSystem/QuestUMG/MainMission.h"
#include "RPG/QuestSystem/QuestUMG/QuestBoard.h"

bool UPlayerControllerMainUI::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	HealthBar = Cast<UGlassBar>(GetWidgetFromName("Health"));
	ManaBar = Cast<UGlassBar>(GetWidgetFromName("Mana"));
	LevelBar = Cast<ULevelBar>(GetWidgetFromName("Level"));
	CharacterBorder = Cast<UCharacterBorder>(GetWidgetFromName("CharacterBorder"));
	MiniMap = Cast<UMiniMap>(GetWidgetFromName("MiniMap"));
	QuestBoard = Cast<UQuestBoard>(GetWidgetFromName("QuestBoard"));
	MainMission = Cast<UMainMission>(GetWidgetFromName("MainMissionBoard"));
	if(MainMission)
	{
		MainMission->SetVisibility(ESlateVisibility::Collapsed);
		MainMission->SetColorAndOpacity(FLinearColor(1,1,1,0));
	}
	SkillBar = Cast<USkillBar>(GetWidgetFromName("SkillBar"));
	SelectTalkInfoBox = Cast<USelectTalkBox>(GetWidgetFromName("SelectTalkInfoBox"));
	TalkBoard = Cast<UTalkBoard>(GetWidgetFromName("TalkBoard"));
	GameMenu = Cast<UGameMenu>(GetWidgetFromName("GameMenu"));
	if(GameMenu)
	{
		GameMenu->SetVisibility(ESlateVisibility::Collapsed);
		GameMenu->SetPCMainUI(this);
	}
	GameOverMenu = Cast<UGameOverMenu>(GetWidgetFromName("GameOverMenu"));
	if(GameOverMenu)
	{
		GameOverMenu->SetVisibility(ESlateVisibility::Collapsed);
		GameOverMenu->SetColorAndOpacity(FLinearColor(1,1,1,0));
	}
		
	//获取动画
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	if(WidgetClass)
	{
		//AnimationMap的KeyValue:”OpenMainMissionBoard_INST“
		for (int i = 0; i < WidgetClass->Animations.Num(); i++)
		{
			FString AnimationName;
			WidgetClass->Animations[i]->GetName(AnimationName);
			WidgetAnimationMap.Add(AnimationName,WidgetClass->Animations[i]);
		}
	}
	
	return true;
}

#pragma region Get/Set

UGlassBar* UPlayerControllerMainUI::GetHealthBar() const
{
	return HealthBar;
}

UGlassBar* UPlayerControllerMainUI::GetManaBar() const
{
	return ManaBar;
}

ULevelBar* UPlayerControllerMainUI::GetLevelBar() const
{
	return LevelBar;
}

UCharacterBorder* UPlayerControllerMainUI::GetCharacterBorder() const
{
	return CharacterBorder;
}

UMiniMap* UPlayerControllerMainUI::GetMiniMap() const
{
	return MiniMap;
}

UQuestBoard* UPlayerControllerMainUI::GetQuestBoard() const
{
	return QuestBoard;
}

USkillBar* UPlayerControllerMainUI::GetSkillBar() const
{
	return SkillBar;
}

UMainMission* UPlayerControllerMainUI::GetMainMission() const
{
	return MainMission;
}

USelectTalkBox* UPlayerControllerMainUI::GetSelectTalkBox() const
{
	return SelectTalkInfoBox;
}

UTalkBoard* UPlayerControllerMainUI::GetTalkBoard() const
{
	return TalkBoard;
}

UGameMenu* UPlayerControllerMainUI::GetGameMenu() const
{
	return GameMenu;
}

UGameOverMenu* UPlayerControllerMainUI::GetGameOverMenu() const
{
	return GameOverMenu;
}

#pragma endregion 

void UPlayerControllerMainUI::SwitchMainMissionBorder()
{
	if(MainMission && WidgetAnimationMap.Contains("OpenMainMissionBoard_INST"))
	{
		if(MainMission->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayAnimation(WidgetAnimationMap["OpenMainMissionBoard_INST"]);
		}
		else if(MainMission->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		{
			PlayAnimation(WidgetAnimationMap["OpenMainMissionBoard_INST"],0,1,EUMGSequencePlayMode::Reverse,1);
		}
	}
}

void UPlayerControllerMainUI::SwitchGameMenu()
{
	if(GameMenu && WidgetAnimationMap.Contains("OpenGameMenu_INST"))
	{
		if(GameMenu->GetVisibility() == ESlateVisibility::Collapsed)
		{
			UGameplayStatics::GetPlayerController(GetWorld(),0)->SetShowMouseCursor(true);
			UGameplayStatics::SetGamePaused(GetWorld(),true);
			PlayAnimation(WidgetAnimationMap["OpenGameMenu_INST"]);
		}
		else if(GameMenu->GetVisibility() == ESlateVisibility::SelfHitTestInvisible)
		{
			UGameplayStatics::GetPlayerController(GetWorld(),0)->SetShowMouseCursor(false);
			UGameplayStatics::SetGamePaused(GetWorld(),false);
			PlayAnimation(WidgetAnimationMap["OpenGameMenu_INST"],0,1,EUMGSequencePlayMode::Reverse,1);
		}
	}
}

void UPlayerControllerMainUI::GameOver()
{
	if(GameOverMenu && WidgetAnimationMap.Contains("GameOver_INST"))
	{
		if(GameOverMenu->GetVisibility() == ESlateVisibility::Collapsed)
		{
			PlayAnimation(WidgetAnimationMap["GameOver_INST"]);
		}
	}
}
