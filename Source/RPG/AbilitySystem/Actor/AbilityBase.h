// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilityBase.generated.h"

UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	Standard,
	Skill,
};

UENUM(BlueprintType)
enum class EAbilityConsumeType : uint8
{
	None,
	Health,
	Mana,
	Stamina
};

USTRUCT(BlueprintType)
struct FAbilityConsume
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EAbilityConsumeType AbilityConsumeType;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float ConsumeValue;
};

USTRUCT(BlueprintType)
struct FAbilityData
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FString Name;//技能名,技能名字不能为“”,也不能重复,用于释放技能查找
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	EAbilityType AbilityType;//技能类型

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	bool bIsCombo;//是否可以连击
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float CoolDownTime;//冷却时间
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float DamageValue;//伤害
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float AbilityDuration;//技能持续时间
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float EffectInterval;//释放效果触发间隔
	
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FAbilityConsume AbilityConsume;//技能消耗

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FString NextAbilityName;//下一个技能名
};

UCLASS()
class RPG_API AAbilityBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbilityBase(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	FAbilityData AbilityData;

	UPROPERTY()
	class ARPGameCharacter* AbilityOwner;

	UPROPERTY()
	class URPGameCharacterAnim* OwnerAnimInstance;
	
	UPROPERTY()
	class UAnimMontage* AbilityAnimMontage;

	UPROPERTY()
	UMaterialInstance* UMGIconMaterialInstance;

	UPROPERTY()
	class UMaterialInstanceDynamic* SkillImageDynamic;

	UPROPERTY()
	class UTextBlock* SkillCDText;
	
	float AbilityTickTime;//技能期间，计时器刷新间隔
	float AbilityAnimRemainTime;//动画剩余时间
	float AbilityAnimEnterNextComboTime;//动画进入NextCombo状态的时间
	/* 动画剩余时间到AbilityAnimEndTime时，结束技能，进入NoCombo状态,
	为0时，在播放完动画结束技能，为负数时，播放完动画延长时间结束技能 */
	float AbilityAnimEndTime;

	
	float CoolDownRemainTime;//冷却剩余时间
	bool bIsCoolDown;
	
	FTimerHandle TimeCoolDownHandle;//冷却计时器
	FTimerHandle AnimTimeCountDownHandle;//动画播放时间的倒计时计时器

	//进入冷却时间
	void EnterTimeCoolDown();
	//动画播放后倒计时
	void PlayAbilityAnim();
	//普通技能动画时间开始倒计时
	void AbilityAnimTimeStartCountDown();
	//可连击技能动画时间开始倒计时
	void ComboAbilityAnimTimeStartCountDown();
	//技能开始时
	virtual void AbilityStart();
	//技能结束时
	virtual void AbilityEnd();
	//技能每帧触发
	virtual void AbilityTick(float DeltaTickTime);
public:
	FString GetAbilityName() const;
	//激活技能
	void ActivateAbility();
	void PauseAnimTimeCountDownHandle();
	virtual void AbilityDamageEffect(class IAbilityAttackInterface* DamageInterface);
	virtual void AbilityVfxEffect(class IAbilityAttackInterface* DamageInterface);
	
	void SetAbilityOwner(class ARPGameCharacter* Character);
	void SetSkillImageDynamic(class UMaterialInstanceDynamic* MaterialInstanceDynamic);
	void SetSkillCDText(class UTextBlock* TextBlock);
	bool CanActivateAbility() const;//判断技能是否可用
	FString GetNextAbilityName() const;//获取技能连续技的下一个技能名字
	EAbilityType GetAbilityType() const;//获取技能类型
	UMaterialInstance* GetUMGIconMaterialInstance() const;
};
