// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerControllerMainUI.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UPlayerControllerMainUI : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<FString,UWidgetAnimation*> WidgetAnimationMap;

	UPROPERTY()
	class UGlassBar* HealthBar;

	UPROPERTY()
	class UGlassBar* ManaBar;

	UPROPERTY()
	class ULevelBar* LevelBar;

	UPROPERTY()
	class UCharacterBorder* CharacterBorder;

	UPROPERTY()
	class UMiniMap* MiniMap;
	
	//任务
	UPROPERTY()
	class UQuestBoard* QuestBoard;

	UPROPERTY()
	class UMainMission* MainMission;

	//技能
	UPROPERTY()
	class USkillBar* SkillBar;

	//菜单
	UPROPERTY()
	class UGameMenu* GameMenu;

	UPROPERTY()
	class UGameOverMenu* GameOverMenu;

	//对话
	UPROPERTY()
	class USelectTalkBox* SelectTalkInfoBox;

	UPROPERTY()
	class UTalkBoard* TalkBoard;

	virtual bool Initialize() override;
public:
	UGlassBar* GetHealthBar() const;
	UGlassBar* GetManaBar() const;
	ULevelBar* GetLevelBar() const;
	UCharacterBorder* GetCharacterBorder() const;
	UMiniMap* GetMiniMap() const;
	UQuestBoard* GetQuestBoard() const;
	USkillBar* GetSkillBar() const;
	UMainMission* GetMainMission() const;
	USelectTalkBox* GetSelectTalkBox() const;
	UTalkBoard* GetTalkBoard() const;
	UGameMenu* GetGameMenu() const;
	UGameOverMenu* GetGameOverMenu() const;

	void SwitchMainMissionBorder();
	void SwitchGameMenu();
	void GameOver();
};
