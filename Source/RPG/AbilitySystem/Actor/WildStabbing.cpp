// Fill out your copyright notice in the Description page of Project Settings.


#include "WildStabbing.h"

#include "RPG/Player/Character/RPGameCharacter.h"

AWildStabbing::AWildStabbing(const FObjectInitializer& ObjectInitializer) : AAbilityBase(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_HealthRegen(TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_WildStabbing.AM_WildStabbing'"));
	if (AM_HealthRegen.Succeeded())
	{
		AbilityAnimMontage = AM_HealthRegen.Object;
	}
	AbilityData.Name = "WildStabbing";
	AbilityData.AbilityType = EAbilityType::Skill;//技能类型
	AbilityData.bIsCombo = false;//是否可以连击
	AbilityData.CoolDownTime = 10;//冷却时间
	AbilityData.DamageValue = 30;//伤害
	AbilityData.AbilityDuration = 0;//技能持续时间
	AbilityData.EffectInterval = 0;//释放效果触发间隔
	AbilityData.AbilityConsume.AbilityConsumeType = EAbilityConsumeType::Mana;//技能消耗
	AbilityData.AbilityConsume.ConsumeValue = 80;
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> Inst_SkillCD(TEXT("MaterialInstanceConstant'/Game/RPG_Game/Materials/Inst_WildStabbing.Inst_WildStabbing'"));
	if(Inst_SkillCD.Succeeded())
	{
		UMGIconMaterialInstance = Inst_SkillCD.Object;
	}
}

void AWildStabbing::AbilityStart()
{
	Super::AbilityStart();
	AbilityOwner->SetPlayerLookAtView();
}
