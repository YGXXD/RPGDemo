// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StateBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UStateBar : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	class UTextBlock* StateText;

	UPROPERTY()
	class UProgressBar* StateProgressBar;
private:
	virtual bool Initialize() override;
public:
	void UpdateState(float CurrentValue, float MaxValue);
};
