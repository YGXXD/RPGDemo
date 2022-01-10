// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBar.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool ULevelBar::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	bCanOpenXpBar = true;
	
	XpProgressBar = Cast<UProgressBar>(GetWidgetFromName("XpProgressBar"));
	if(XpProgressBar)
	{
		XpProgressBar->SetPercent(0);
		XpProgressBar->SetRenderScale(FVector2D(0,1));
	}
	
	LevelNumberText = Cast<UTextBlock>(GetWidgetFromName("LevelNumberText"));
	if(LevelNumberText)
	{
		LevelNumberText->SetText(FText::FromName(TEXT("")));
	}
	
	//获取动画
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	if(WidgetClass && WidgetClass->Animations.Num())
	{
		XpBarToViewAnimation = WidgetClass->Animations[0];
	}
	
	return true;
}

// 更新UI的逻辑已经在RPGGameCharacter中实现
// void ULevelBar::GetExperience(float Experience)
// {
// 	//显示经验条
// 	if(Experience <= 0)
// 	{
// 		return;
// 	}
// 	XpBarToView();
// 	CurrentXp += Experience;
// 	auto XpUpdateLambda = [this]()
// 	{
// 		float XpPercent = XpProgressBar->Percent;
// 		XpPercent = FMath::Clamp<float>(XpPercent + 0.003,0,CurrentXp/MaxXp);
// 		XpProgressBar->SetPercent(XpPercent);
//
// 		//判断是否升级了
// 		if(XpPercent >= 1)
// 		{
// 			XpProgressBar->SetPercent(0);
// 			CurrentXp -= MaxXp;
// 			MaxXp *= 1.5;
// 		}
// 		
// 		if(XpPercent == CurrentXp/MaxXp)
// 		{
// 			//延时隐藏经验条
// 			DelayXpBarToHide(10);
// 			GetWorld()->GetTimerManager().PauseTimer(XpUpdateHandle);
// 		}
// 	};
// 	const FTimerDelegate XpUpdateDelegate = FTimerDelegate::CreateLambda(XpUpdateLambda);
// 	GetWorld()->GetTimerManager().SetTimer(XpUpdateHandle,XpUpdateDelegate,0.01,true);
// 	
// }

void ULevelBar::SetXpPercent(float PercentValue)
{
	if(XpProgressBar)
		XpProgressBar->SetPercent(PercentValue);
}

void ULevelBar::XpBarToView()
{
	if(bCanOpenXpBar)
	{
		PlayAnimation(XpBarToViewAnimation);
		bCanOpenXpBar = false;
	}
}

void ULevelBar::XpBarToHide()
{
	//循环函数计时器停止工作时，才能隐藏XpBar
	if(GetWorld()->GetTimerManager().IsTimerPaused(XpUpdateHandle)&&!bCanOpenXpBar)
	{
		PlayAnimation(XpBarToViewAnimation,0,1,EUMGSequencePlayMode::Reverse,1);
		bCanOpenXpBar = true;
	}
}

void ULevelBar::DelayXpBarToHide(float DelayTime)
{
	auto XpBarToHideLambda = [this]()
	{
		XpBarToHide();
	};
	const FTimerDelegate XpBarToHideDelegate = FTimerDelegate::CreateLambda(XpBarToHideLambda);
	GetWorld()->GetTimerManager().SetTimer(HideXpBarHandle,XpBarToHideDelegate,DelayTime,false);
}

float ULevelBar::GetXpPercent() const
{
	if(XpProgressBar)
		return XpProgressBar->Percent;
	return 0;
}

void ULevelBar::SetUpdateTimer(FTimerDelegate const& InDelegate, float InRate, bool InbLoop)
{
	GetWorld()->GetTimerManager().SetTimer(XpUpdateHandle,InDelegate,InRate,InbLoop);
}

void ULevelBar::PauseUpdateTimer()
{
	GetWorld()->GetTimerManager().PauseTimer(XpUpdateHandle);
}

void ULevelBar::UpdateLevelValue(float CurrentValue)
{
	if(LevelNumberText)
		LevelNumberText->SetText(FText::AsNumber(static_cast<INT>(CurrentValue)));
}