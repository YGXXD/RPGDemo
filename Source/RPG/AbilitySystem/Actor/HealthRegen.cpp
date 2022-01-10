// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthRegen.h"

#include "RPG/Player/Character/RPGameCharacter.h"

AHealthRegen::AHealthRegen(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HealthRegen(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_HealthRegen.AM_HealthRegen'"));
	if (AM_HealthRegen.Succeeded())
	{
		AbilityAnimMontage = AM_HealthRegen.Object;
	}
	AbilityData.Name = "HealthRegen";
	AbilityData.AbilityType = EAbilityType::Skill;//技能类型
	AbilityData.bIsCombo = false;//是否可以连击
	AbilityData.CoolDownTime = 7;//冷却时间
	AbilityData.DamageValue = 30;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::Mana;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 200;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Inst_SkillCD(TEXT("MaterialInstanceConstant'/Game/RPG_Game/Materials/Inst_HealthRegen.Inst_HealthRegen'"));
	if(Inst_SkillCD.Succeeded())
	{
		UMGIconMaterialInstance = Inst_SkillCD.Object;
	}
}

void AHealthRegen::AbilityTick(float DeltaTickTime)
{
	Super::AbilityTick(DeltaTickTime);
	AbilityOwner->ModifyAttribute(EAttributeType::Health,3);
}
