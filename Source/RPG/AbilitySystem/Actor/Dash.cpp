// Fill out your copyright notice in the Description page of Project Settings.


#include "Dash.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"

ADash::ADash(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Dash(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_Dash.AM_Dash'"));
	if (AM_Dash.Succeeded())
	{
		AbilityAnimMontage = AM_Dash.Object;
	}
	AbilityAnimEndTime = 1.2;
	AbilityData.Name = "Dash";
	AbilityData.AbilityType = EAbilityType::Standard;//技能类型
	AbilityData.bIsCombo = false;//是否可以连击
	AbilityData.CoolDownTime = 0;//冷却时间
	AbilityData.DamageValue = 0;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::Stamina;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 30;
}

void ADash::AbilityStart()
{
	Super::AbilityStart();
	if(AbilityOwner)
	{
		AbilityOwner->DisableControllerInput();
		const FVector ForwardVector = AbilityOwner->GetActorForwardVector();
		AbilityOwner->GetPlayerMovementComponent()->AddImpulse(ForwardVector * 7000,true);
	}
}

void ADash::AbilityEnd()
{
	Super::AbilityEnd();
	if(AbilityOwner)
	{
		AbilityOwner->EnableControllerInput();
	}
}

