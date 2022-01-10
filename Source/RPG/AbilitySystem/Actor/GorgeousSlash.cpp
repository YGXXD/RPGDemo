// Fill out your copyright notice in the Description page of Project Settings.


#include "GorgeousSlash.h"

#include "RPG/Player/Character/RPGameCharacter.h"

AGorgeousSlash::AGorgeousSlash(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HealthRegen(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_GorgeousSlash.AM_GorgeousSlash'"));
	if (AM_HealthRegen.Succeeded())
	{
		AbilityAnimMontage = AM_HealthRegen.Object;
	}
	AbilityData.Name = "GorgeousSlash";
	AbilityData.AbilityType = EAbilityType::Skill;//技能类型
	AbilityData.bIsCombo = false;//是否可以连击
	AbilityData.CoolDownTime = 10;//冷却时间
	AbilityData.DamageValue = 30;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::Mana;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 80;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Inst_SkillCD(TEXT("MaterialInstanceConstant'/Game/RPG_Game/Materials/Inst_GorgeousSlash.Inst_GorgeousSlash'"));
	if(Inst_SkillCD.Succeeded())
	{
		UMGIconMaterialInstance = Inst_SkillCD.Object;
	}
}

void AGorgeousSlash::AbilityStart()
{
	Super::AbilityStart();
	AbilityOwner->SetPlayerLookAtView();
}
