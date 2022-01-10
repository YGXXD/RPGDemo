// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeA.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"


AMeleeA::AMeleeA(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_MeleeA(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_MeleeA.AM_MeleeA'"));
	if (AM_MeleeA.Succeeded())
	{
		AbilityAnimMontage = AM_MeleeA.Object;
	}
	AbilityAnimEnterNextComboTime = 0.9;
	AbilityData.Name = "MeleeA";
	AbilityData.AbilityType = EAbilityType::Standard;//技能类型
	AbilityData.bIsCombo = true;//是否可以连击
	AbilityData.CoolDownTime = 0;//冷却时间
	AbilityData.DamageValue = 250;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::None;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 0;
	AbilityData.NextAbilityName = "MeleeB";//下一个技能名
}

void AMeleeA::AbilityStart()
{
	Super::AbilityStart();
	AbilityOwner->SetPlayerLookAtView();
}

void AMeleeA::AbilityVfxEffect(IAbilityAttackInterface* DamageInterface)
{
	Super::AbilityVfxEffect(DamageInterface);
}
