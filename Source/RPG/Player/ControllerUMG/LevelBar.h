// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ULevelBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	FTimerHandle XpUpdateHandle;
	FTimerHandle HideXpBarHandle;
	
	bool bCanOpenXpBar;
	
	UPROPERTY()
	class UProgressBar* XpProgressBar;

	UPROPERTY()
	class UTextBlock* LevelNumberText;

	UPROPERTY()
	class UWidgetAnimation* XpBarToViewAnimation;

	virtual bool Initialize() override;

public:
	// 更新UI的逻辑已经在RPGGameCharacter中实现
	// void GetExperience(float Experience);
	
	UFUNCTION()
	void XpBarToView();
	
	UFUNCTION()
	void XpBarToHide();
	
	//延迟隐藏经验条
	void DelayXpBarToHide(float DelayTime);
	//获取经验条百分比值
	float GetXpPercent() const;
	void SetXpPercent(float PercentValue);
	//设置Xp更新的计时器
	void SetUpdateTimer(FTimerDelegate const& InDelegate, float InRate, bool InbLoop);
	void PauseUpdateTimer();
	//更新Level的Text
	void UpdateLevelValue(float CurrentValue);
};
