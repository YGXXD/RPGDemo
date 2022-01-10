// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TalkBoard.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UTalkBoard : public UUserWidget
{
	GENERATED_BODY()
private:
	
	UPROPERTY()
	class UTextBlock* NameText;

	UPROPERTY()
	class UTextBlock* TalkInfoText;

	virtual bool Initialize() override;
public:
	void ShowTalkInfo(FString Name, FString TalkInfo);
	void HideTalkInfo();
};
