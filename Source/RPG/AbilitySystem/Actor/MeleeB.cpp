// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeB.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"


AMeleeB::AMeleeB(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_MeleeB(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_MeleeB.AM_MeleeB'"));
	if (AM_MeleeB.Succeeded())
	{
		AbilityAnimMontage = AM_MeleeB.Object;
	}
	AbilityAnimEnterNextComboTime = 0.8;
	AbilityData.Name = "MeleeB";
	AbilityData.AbilityType = EAbilityType::Standard;//技能类型
	AbilityData.bIsCombo = true;//是否可以连击
	AbilityData.CoolDownTime = 0;//冷却时间
	AbilityData.DamageValue = 30;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::None;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 0;
	AbilityData.NextAbilityName = "MeleeC";//下一个技能名
}

void AMeleeB::AbilityStart()
{
	Super::AbilityStart();
	AbilityOwner->SetPlayerLookAtView();
}

void AMeleeB::AbilityVfxEffect(IAbilityAttackInterface* DamageInterface)
{
	Super::AbilityVfxEffect(DamageInterface);
}
