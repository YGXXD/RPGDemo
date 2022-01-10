// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStateBar.h"

#include "Components/ProgressBar.h"

bool UEnemyStateBar::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	BackStateBar = Cast<UProgressBar>(GetWidgetFromName("BackStateBar"));
	FrontStateBar = Cast<UProgressBar>(GetWidgetFromName("FrontStateBar"));

	return true;
}

void UEnemyStateBar::SetEnemyStateBar(float CurrentValue, float MaxValue)
{
	SavePercent = CurrentValue/MaxValue;
	BackStateBar->SetPercent(SavePercent);
	FrontStateBar->SetPercent(SavePercent);
}

void UEnemyStateBar::UpdateEnemyStateBar(float CurrentValue, float MaxValue)
{
	TargetPercent = CurrentValue/MaxValue;
	if(SavePercent < TargetPercent)//血量增加了
	{
		UE_LOG(LogTemp,Warning,TEXT("%f"),TargetPercent)
		SavePercent = TargetPercent;
		BackStateBar->SetPercent(SavePercent);
		FrontStateBar->SetPercent(SavePercent);
	}
	else
	{
		FrontStateBar->SetPercent(TargetPercent);
		GetWorld()->GetTimerManager().PauseTimer(DelayUpdateHandle);
		auto DelayUpdateLambda = [this]()
		{
			auto UpdateStateLambda = [this]()
			{
				if(BackStateBar->Percent > TargetPercent)
				{
					SavePercent = BackStateBar->Percent - 0.01;
					BackStateBar->SetPercent(SavePercent);
				}
				else
				{
					GetWorld()->GetTimerManager().PauseTimer(UpdateStateHandle);
				}
			};
			const FTimerDelegate UpdateStateDelegate = FTimerDelegate::CreateLambda(UpdateStateLambda);
			GetWorld()->GetTimerManager().SetTimer(UpdateStateHandle,UpdateStateDelegate,0.02,true);
		};
		const FTimerDelegate DelayUpdateDelegate = FTimerDelegate::CreateLambda(DelayUpdateLambda);
		GetWorld()->GetTimerManager().SetTimer(DelayUpdateHandle,DelayUpdateDelegate,2,false);
	}
}
