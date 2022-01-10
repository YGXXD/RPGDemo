// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG/NeutralObject/Character/NPCharacterBase.h"
#include "SelectTalkBox.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API USelectTalkBox : public UUserWidget
{
	GENERATED_BODY()

private:
	int DialogueTalkContentID;
	FTimerHandle PlayDialogueHandle;
	int CurrentDialogueIndex;
	FDialogueContent CurrentDialogue;

	UPROPERTY()
	TSubclassOf<class USelectTalkInfo> SelectTalkInfoClass;
	
	UPROPERTY()
	class ARPGameCharacter* Player;
	
	UPROPERTY()
	class ANPCharacterBase* NPCharacter;
	
	UPROPERTY()
	class UScrollBox* SelectTalkInfoScrollBox;

	UPROPERTY()
	class UTalkBoard* TalkBoard;

	virtual bool Initialize() override;
	void UpdateSelectTalkBox();
	
	void AutoPlayDialogue();
	//对话细节与对话选项实现
	void SelectElseInfo();
	void SelectQuestInfo();
	void SelectEquipStore();
	void SelectDrugStore();
	void SelectFoodStore();
	void SelectEnd();


public:
	void SetPlayer(class ARPGameCharacter* const Character);
	//对话选择后继续
	void SelectTalkInfo(int Index);
	void TalkContinue();
	void TalkBack();
	void TalkAcceptQuest();
	void TalkQuit();

	void SetUpCharacterToUpdate(class ANPCharacterBase* const Character);
	void SetTalkBoard(class UTalkBoard* const Board);
	
};
