// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NeutralObjectInteraction.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UNeutralObjectInteraction : public UUserWidget
{
	GENERATED_BODY()

private:
	bool bIsInteracting;
	FTimerHandle HideTalkInfoTextHandle;
	
	UPROPERTY()
	class UTextBlock* NameText;

	UPROPERTY()
	class UTextBlock* InteractionText;
	
	UPROPERTY()
	class UTextBlock* TalkInfoText;

	virtual bool Initialize() override;
public:
	void SetNameText(FString Name);
	void ShowTalkInfo(FText TalkInfo);
};
