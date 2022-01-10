// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeC.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"

AMeleeC::AMeleeC(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_MeleeC(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_MeleeC.AM_MeleeC'"));
	if (AM_MeleeC.Succeeded())
	{
		AbilityAnimMontage = AM_MeleeC.Object;
	}
	AbilityAnimEndTime = 0.8;
	AbilityData.Name = "MeleeC";
	AbilityData.AbilityType = EAbilityType::Standard;//技能类型
	AbilityData.bIsCombo = false;//是否可以连击
	AbilityData.CoolDownTime = 0;//冷却时间
	AbilityData.DamageValue = 35;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::None;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 0;
}

void AMeleeC::AbilityStart()
{
	Super::AbilityStart();
	AbilityOwner->SetPlayerLookAtView();
}

void AMeleeC::AbilityVfxEffect(IAbilityAttackInterface* DamageInterface)
{
	Super::AbilityVfxEffect(DamageInterface);
}