// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeInAirB.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"

AMeleeInAirB::AMeleeInAirB(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_MeleeInAir(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_MeleeAirB.AM_MeleeAirB'"));
	if (AM_MeleeInAir.Succeeded())
	{
		AbilityAnimMontage = AM_MeleeInAir.Object;
	}
	AbilityAnimEnterNextComboTime = 1.3;
	AbilityData.Name = "MeleeInAirB";
	AbilityData.AbilityType = EAbilityType::Standard;//技能类型
	AbilityData.bIsCombo = true;//是否可以连击
	AbilityData.CoolDownTime = 0;//冷却时间
	AbilityData.DamageValue = 30;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::Stamina;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 10;
	AbilityData.NextAbilityName = "MeleeInAirC";
}

void AMeleeInAirB::AbilityStart()
{
	Super::AbilityStart();
	AbilityOwner->SetPlayerLookAtView();
	AbilityOwner->LaunchCharacter(FVector(0,0,300),false,true);
}

void AMeleeInAirB::AbilityEnd()
{
	Super::AbilityEnd();
}

void AMeleeInAirB::AbilityVfxEffect(IAbilityAttackInterface* DamageInterface)
{
	Super::AbilityVfxEffect(DamageInterface);
}
