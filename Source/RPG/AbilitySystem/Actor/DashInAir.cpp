// Fill out your copyright notice in the Description page of Project Settings.


#include "DashInAir.h"

// Fill out your copyright notice in the Description page of Project Settings.


#include "Dash.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"

ADashInAir::ADashInAir(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Dash(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_DashAir.AM_DashAir'"));
	if (AM_Dash.Succeeded())
	{
		AbilityAnimMontage = AM_Dash.Object;
	}
	AbilityAnimEndTime = 1.5;
	AbilityData.Name = "DashInAir";
	AbilityData.AbilityType = EAbilityType::Standard;//技能类型
	AbilityData.bIsCombo = false;//是否可以连击
	AbilityData.CoolDownTime = 0;//冷却时间
	AbilityData.DamageValue = 0;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::Stamina;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 30;
}

void ADashInAir::AbilityStart()
{
	Super::AbilityStart();
	if(AbilityOwner)
	{
		AbilityOwner->SetPlayerLookAtView();
		AbilityOwner->LaunchCharacter(FVector(0,0,300),false,true);
	}
}

void ADashInAir::AbilityEnd()
{
	Super::AbilityEnd();
}

