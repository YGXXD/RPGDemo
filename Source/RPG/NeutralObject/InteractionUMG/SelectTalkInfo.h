// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG/NeutralObject/Character/NPCharacterBase.h"
#include "SelectTalkInfo.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API USelectTalkInfo : public UUserWidget
{
	GENERATED_BODY()

private:
	FString SelectTalkInfoString;
	int TalkIndex;
	ESubTalkType SubTalkType;
	
	UPROPERTY()
	class USelectTalkBox* SelectTalkBox;

	//控件
	UPROPERTY()
	class UImage* TalkTypeImage;
	
	UPROPERTY()
	class UTextBlock* SelectTalkInfoText;

	UPROPERTY()
	class UButton* SelectTalkButton;

	virtual bool Initialize() override;
	UFUNCTION()
	void OnSelectTalkButtonClick();

public:
	void SetSelectTalkBox(class USelectTalkBox* const TalkBox);
	void SetSubTalkType(ESubTalkType Type);
	void SetTalkIndex(int Index);
	void SetSelectTalkInfoString(FString TalkInfo);
	
	void UpdateSelectTalkInfo();
};
