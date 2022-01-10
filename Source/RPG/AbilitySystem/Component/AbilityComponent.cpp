// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

#include "Kismet/GameplayStatics.h"
#include "RPG/AbilitySystem/AbilityUMG/SkillBar.h"
#include "RPG/AbilitySystem/Actor/AbilityBase.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/LoadSystem/SaveData/RPGSaveGame.h"

UAbilityComponent::UAbilityComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	ComboState = EComboState::NoCombo;
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UAbilityComponent::SetPlayer(ARPGameCharacter* const RPGameCharacter)
{
	Player = RPGameCharacter;	
}

void UAbilityComponent::SetComboState(EComboState State)
{
	ComboState = State;
}

void UAbilityComponent::SetCurrentAbility(AAbilityBase* const Ability)
{
	CurrentAbility = Ability;
}

void UAbilityComponent::SetPlayerSkillBar(USkillBar* const SkillBar)
{
	PlayerSkillBar = SkillBar;
}

void UAbilityComponent::TryActivateAbility(FString AbilityName)
{
	if(ComboState == EComboState::InCombo)
	{
		return;
	}
	//判断有没有这个技能
	if(AbilityMap.Contains(AbilityName))//有技能则释放
	{
		if (ComboState == EComboState::NoCombo)
		{
			if (AbilityMap[AbilityName]->CanActivateAbility())
			{
				//储存技能名字
				SaveAbilityName = AbilityName;
				SetCurrentAbility(AbilityMap[AbilityName]);
				CurrentAbility->ActivateAbility();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("无法释放该技能"));
			}
		}
		else if (ComboState == EComboState::CanEnterNextCombo)
		{
			//技能连击逻辑
			CurrentAbility->PauseAnimTimeCountDownHandle();
			const FString NextAbilityName = CurrentAbility->GetNextAbilityName();
			if (AbilityMap.Contains(NextAbilityName) && SaveAbilityName == AbilityName)
			{
				/* SaveAbilityName == AbilityName为false,则在CanEnterNextCombo期间,
				输入的技能名字改变,就不执行下一次连击,而是执行新输入的技能。
				
				AbilityMap.Contains(NextAbilityName)为false,则连击技能到最后了,没有后续技能,
				输入的AbilityName为连击开始的技能，则执行连击开始的技能 */
				if(AbilityMap[NextAbilityName]->CanActivateAbility())
				{
					SetCurrentAbility(AbilityMap[NextAbilityName]);
					CurrentAbility->ActivateAbility();
				}
			}
			else
			{
				if (CurrentAbility)
				{
					CurrentAbility->PauseAnimTimeCountDownHandle();
				}
				if (AbilityMap[AbilityName]->CanActivateAbility())
				{
					SaveAbilityName = AbilityName;
					SetCurrentAbility(AbilityMap[AbilityName]);
					CurrentAbility->ActivateAbility();
				}
			}
		}
	}
	else
	{
		//没有技能则输出信息
		UE_LOG(LogTemp,Warning,TEXT("没有学习该技能"));
	}
}

void UAbilityComponent::AddAbility(TSubclassOf<AAbilityBase> AbilityClass, bool bIsBirthAbility)
{
	if(!AllAbilityClassList.Contains(AbilityClass) && AbilityClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		AAbilityBase* Ability = GetWorld()->SpawnActor<AAbilityBase>(AbilityClass,SpawnParams);
		Ability->SetAbilityOwner(Player);
		AllAbilityClassList.Add(AbilityClass);
		if(!bIsBirthAbility)
		{
			LaterAbilityClassList.Add(AbilityClass);
		}
		AbilityMap.Add(Ability->GetAbilityName(),Ability);
		if(Ability->GetAbilityType() == EAbilityType::Skill)
		{
			PlayerSkillBar->AddSkillToSkillBar(Ability);
		}
	}
}

#pragma region 保存技能

void UAbilityComponent::SaveAbility() const
{
	SAVE_DATA->SavePlayerAbility(LaterAbilityClassList);
}

void UAbilityComponent::LoadAbility(int Index)
{
	const TArray<TSubclassOf<AAbilityBase>> AbilityList = SAVE_DATA->GetPlayerAbilityList(Index);
	for (int i = 0; i < AbilityList.Num(); i++)
	{
		AddAbility(AbilityList[i]);
	}
}

#pragma endregion