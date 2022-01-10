// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyStateBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UEnemyStateBar : public UUserWidget
{
	GENERATED_BODY()

private:
	FTimerHandle DelayUpdateHandle;
	FTimerHandle UpdateStateHandle;
	float SavePercent;
	float TargetPercent;
	
	UPROPERTY()
	class UProgressBar* BackStateBar;

	UPROPERTY()
	class UProgressBar* FrontStateBar;

	virtual bool Initialize() override;

public:
	void SetEnemyStateBar(float CurrentValue, float MaxValue);
	void UpdateEnemyStateBar(float CurrentValue, float MaxValue);
};
