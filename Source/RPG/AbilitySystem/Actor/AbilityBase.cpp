// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityBase.h"

#include "Components/TextBlock.h"
#include "RPG/AbilitySystem/Component/AbilityComponent.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"
#include "RPG/Player/Animation/RPGameCharacterAnim.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/Player/Interface/AbilityAttackInterface.h"


AAbilityBase::AAbilityBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bIsCoolDown = false;
	CoolDownRemainTime = 0;

    AbilityTickTime = 0.02;
	AbilityAnimEnterNextComboTime = 0.5;
	AbilityAnimEndTime = 0;
	AbilityAnimRemainTime = AbilityAnimEndTime;
	
	SkillImageDynamic = nullptr;
}

#pragma region Set/Get

void AAbilityBase::SetAbilityOwner(ARPGameCharacter* Character)
{
	AbilityOwner = Character;
	if(Character)
	{
		OwnerAnimInstance = Cast<URPGameCharacterAnim>(Character->GetMesh()->GetAnimInstance());
	}
}

void AAbilityBase::SetSkillImageDynamic(UMaterialInstanceDynamic* MaterialInstanceDynamic)
{
	SkillImageDynamic = MaterialInstanceDynamic;
	if(SkillImageDynamic && !bIsCoolDown)
	{
		SkillImageDynamic->SetScalarParameterValue(TEXT("Percentage"),0);
	}
}

void AAbilityBase::SetSkillCDText(UTextBlock* TextBlock)
{
	SkillCDText = TextBlock;
}

FString AAbilityBase::GetAbilityName() const
{
	return AbilityData.Name;	
}

FString AAbilityBase::GetNextAbilityName() const
{
	return AbilityData.NextAbilityName;
}

EAbilityType AAbilityBase::GetAbilityType() const
{
	return AbilityData.AbilityType;
}

UMaterialInstance* AAbilityBase::GetUMGIconMaterialInstance() const
{
	return UMGIconMaterialInstance;
}

#pragma endregion 

#pragma region 技能释放开始和结束,和释放效果

void AAbilityBase::ActivateAbility()
{
	PlayAbilityAnim();
	EnterTimeCoolDown();
}

void AAbilityBase::AbilityStart()
{
	bIsCoolDown = true;
	OwnerAnimInstance->SetCurrentAbility(this);
	AbilityOwner->GetAbilityComponent()->SetComboState(EComboState::InCombo);
}

void AAbilityBase::AbilityEnd()
{
	AbilityOwner->GetAbilityComponent()->SetComboState(EComboState::NoCombo);
	AbilityOwner->GetAbilityComponent()->SetCurrentAbility(nullptr);
	GetWorldTimerManager().PauseTimer(AnimTimeCountDownHandle);
}

void AAbilityBase::AbilityTick(float DeltaTickTime)
{
	AbilityAnimRemainTime -= DeltaTickTime;
}

void AAbilityBase::AbilityDamageEffect(class IAbilityAttackInterface* DamageInterface)
{
	if(DamageInterface)
	{
		DamageInterface->TakeDamageForCharacter(AbilityData.DamageValue);
	}
}

void AAbilityBase::AbilityVfxEffect(IAbilityAttackInterface* DamageInterface)
{
	if(DamageInterface)
	{
		DamageInterface->TakeDamageWithEffect();
	}
}

#pragma endregion

#pragma region 技能播放动画和动画倒计时

void AAbilityBase::PlayAbilityAnim()
{
	AbilityStart();
	AbilityAnimRemainTime = PLAY_MONTAGE(OwnerAnimInstance,AbilityAnimMontage);
	if(AbilityData.bIsCombo)
	{
		ComboAbilityAnimTimeStartCountDown();
	}
	else
	{
		AbilityAnimTimeStartCountDown();
	}
}

void AAbilityBase::AbilityAnimTimeStartCountDown()
{
	auto StartAnimRemainTimeLambda = [this]()
	{
		AbilityTick(AbilityTickTime);
		//动画时间小于动画最后时间，EndTime可以自定义，理解为技能结束时间
		if(AbilityAnimRemainTime <= AbilityAnimEndTime)
		{
			AbilityAnimRemainTime = AbilityAnimEndTime;
			AbilityEnd();
		}	
	};
	const FTimerDelegate StartAnimRemainTimeDelegate = FTimerDelegate::CreateLambda(StartAnimRemainTimeLambda);
	GetWorldTimerManager().SetTimer(AnimTimeCountDownHandle,StartAnimRemainTimeDelegate,AbilityTickTime,true);
}

void AAbilityBase::ComboAbilityAnimTimeStartCountDown()
{
	auto StartAnimRemainTimeLambda = [this]()
	{
		AbilityTick(AbilityTickTime);
		//当动画剩余时间小于动画可以进入下一次连击的时间,且技能可以连击执行
		if(AbilityAnimRemainTime < AbilityAnimEnterNextComboTime)
		{
			AbilityOwner->GetAbilityComponent()->SetComboState(EComboState::CanEnterNextCombo);
			
			auto EndAnimRemainTimeLambda = [this]()
			{
				AbilityTick(AbilityTickTime);
				//动画时间小于动画最后时间，EndTime可以自定义，理解为技能结束时间
				if(AbilityAnimRemainTime <= AbilityAnimEndTime)
				{
					AbilityAnimRemainTime = AbilityAnimEndTime;
					AbilityEnd();
				}	
			};
			const FTimerDelegate EndAnimRemainTimeDelegate = FTimerDelegate::CreateLambda(EndAnimRemainTimeLambda);
			GetWorldTimerManager().SetTimer(AnimTimeCountDownHandle,EndAnimRemainTimeDelegate,AbilityTickTime,true);
		}
	};
	const FTimerDelegate StartAnimRemainTimeDelegate = FTimerDelegate::CreateLambda(StartAnimRemainTimeLambda);
	GetWorldTimerManager().SetTimer(AnimTimeCountDownHandle,StartAnimRemainTimeDelegate,AbilityTickTime,true);
}

void AAbilityBase::PauseAnimTimeCountDownHandle()
{
	GetWorldTimerManager().PauseTimer(AnimTimeCountDownHandle);
}

#pragma endregion 

#pragma region 技能CD判断和是否可用

void AAbilityBase::EnterTimeCoolDown()
{
	CoolDownRemainTime = AbilityData.CoolDownTime;
	if(SkillImageDynamic && SkillCDText)
	{
		SkillCDText->SetText(FText::AsNumber(static_cast<int>(AbilityData.CoolDownTime)));
		SkillCDText->SetVisibility(ESlateVisibility::Visible);
		SkillImageDynamic->SetScalarParameterValue(TEXT("Percentage"),1);
	}
	auto CalculateCoolDownLambda = [this]()
	{
		CoolDownRemainTime -= 0.02;
		//UMG的材质实例变化
		if(SkillImageDynamic && SkillCDText)
		{
			SkillCDText->SetText(FText::AsNumber(static_cast<int>(CoolDownRemainTime)));
			SkillImageDynamic->SetScalarParameterValue(TEXT("Percentage"),CoolDownRemainTime/AbilityData.CoolDownTime);
		}
		if(CoolDownRemainTime <= 0)
		{
			if(SkillImageDynamic && SkillCDText)
			{
				SkillCDText->SetVisibility(ESlateVisibility::Collapsed);
				SkillImageDynamic->SetScalarParameterValue(TEXT("Percentage"),0);
			}
			CoolDownRemainTime = 0;
			bIsCoolDown = false;
			GetWorldTimerManager().PauseTimer(TimeCoolDownHandle);
		}
	};
	const FTimerDelegate CalculateCoolDownDelegate = FTimerDelegate::CreateLambda(CalculateCoolDownLambda);
	GetWorldTimerManager().SetTimer(TimeCoolDownHandle,CalculateCoolDownDelegate,0.02,true);
}

bool AAbilityBase::CanActivateAbility() const
{
	const FAbilityConsume AbilityConsume = AbilityData.AbilityConsume;
	switch (AbilityConsume.AbilityConsumeType)
	{
	case EAbilityConsumeType::None:
		return !bIsCoolDown;
		
	case EAbilityConsumeType::Health:
		if(AbilityOwner->GetCurrentHealth() >= AbilityConsume.ConsumeValue && !bIsCoolDown)
		{
			AbilityOwner->ModifyAttribute(EAttributeType::Health,-AbilityConsume.ConsumeValue);
			return true;
		}
		break;
		
	case EAbilityConsumeType::Mana:
		if(AbilityOwner->GetCurrentMana() >= AbilityConsume.ConsumeValue && !bIsCoolDown)
		{
			AbilityOwner->ModifyAttribute(EAttributeType::Mana,-AbilityConsume.ConsumeValue);
			return true;
		}
		break;

	case EAbilityConsumeType::Stamina:
		if(AbilityOwner->GetCurrentStamina() >= AbilityConsume.ConsumeValue && !bIsCoolDown)
		{
			AbilityOwner->ModifyAttribute(EAttributeType::Stamina,-AbilityConsume.ConsumeValue);
			return true;
		}
		break;
		
	default:
		break;
	}
	return false;
}

#pragma endregion