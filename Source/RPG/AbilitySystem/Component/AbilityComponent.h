// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AbilityComponent.generated.h"

UENUM()
enum class EComboState : uint8
{
	NoCombo,//没有连击，释放第一段技能
	InCombo,//正在连击，但是不能释放其他技能
	CanEnterNextCombo //可以释放下一段技能
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RPG_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	EComboState ComboState;

	//用来储存释放技能的名字,用于判断连击中每次输入的的名字是否相等
	FString SaveAbilityName;
	
	UPROPERTY()//技能连击时用来储存当前技能
	class AAbilityBase* CurrentAbility;
	
	UPROPERTY()
	class ARPGameCharacter* Player;

	UPROPERTY()
	TArray<TSubclassOf<class AAbilityBase>> AllAbilityClassList;
	
	UPROPERTY()
	TArray<TSubclassOf<class AAbilityBase>> LaterAbilityClassList;

	UPROPERTY()
	TMap<FString, class AAbilityBase*> AbilityMap;

	//技能栏UI
	UPROPERTY()
	class USkillBar* PlayerSkillBar;

protected:
	virtual void BeginPlay() override;
public:
	UAbilityComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void SetPlayer(class ARPGameCharacter* const RPGameCharacter);
	//尝试释放技能
	void TryActivateAbility(FString AbilityName);
	//添加技能
	void AddAbility(TSubclassOf<class AAbilityBase> AbilityClass ,bool bIsBirthAbility = false);
	void SetComboState(EComboState State);
	void SetCurrentAbility(class AAbilityBase* const Ability);
	void SetPlayerSkillBar(class USkillBar* const SkillBar);
	
	//保存技能列表
	void SaveAbility() const;
	void LoadAbility(int Index);
};
