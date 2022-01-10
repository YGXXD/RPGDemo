// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeD.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"

AMeleeD::AMeleeD(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_MeleeC(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_MeleeD.AM_MeleeD'"));
	if (AM_MeleeC.Succeeded())
	{
		AbilityAnimMontage = AM_MeleeC.Object;
	}
	AbilityAnimEnterNextComboTime = 1;
	AbilityData.Name = "MeleeD";
	AbilityData.AbilityType = EAbilityType::Standard;//技能类型
	AbilityData.bIsCombo = true;//是否可以连击
	AbilityData.CoolDownTime = 0;//冷却时间
	AbilityData.DamageValue = 35;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::None;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 0;
	AbilityData.NextAbilityName = "MeleeE";//下一个技能名
}

void AMeleeD::AbilityStart()
{
	Super::AbilityStart();
	AbilityOwner->SetPlayerLookAtView();
}

void AMeleeD::AbilityVfxEffect(IAbilityAttackInterface* DamageInterface)
{
	Super::AbilityVfxEffect(DamageInterface);
}
