// Fill out your copyright notice in the Description page of Project Settings.

#include "RPGSaveGame.h"

#include "RPG/QuestSystem/Actor/MasterQuest.h"


void URPGSaveGame::DeleteDataByIndex(int Index)
{
	if(Index < 0 || Index >= TimeStringArray.Num())
	{
		return;
	}
	TimeStringArray.RemoveAt(Index);
	HealthArray.RemoveAt(Index);
	ManaArray.RemoveAt(Index);
	StaminaArray.RemoveAt(Index);
	ExperienceArray.RemoveAt(Index);
	LevelArray.RemoveAt(Index);
	MoneyArray.RemoveAt(Index);
	PlayerTransformArray.RemoveAt(Index);
	PlayerAbilityListArray.RemoveAt(Index);
	QuestListArray.RemoveAt(Index);
}

void URPGSaveGame::SaveTimeString(const FString Time)
{
	TimeStringArray.Add(Time);
}

TArray<FString> URPGSaveGame::GetTimeArray() const
{
	return TimeStringArray;
}

void URPGSaveGame::SavePlayerTransform(const FTransform& Transform)
{
	PlayerTransformArray.Add(Transform);
}

FTransform URPGSaveGame::GetPlayerTransform(int Index) const
{
	if(Index >= PlayerTransformArray.Num())
		return FTransform();
	return PlayerTransformArray[Index];
}

void URPGSaveGame::SavePlayerAttribute(const TMap<EAttributeType, FAttribute>& AttributeMap)
{
	FCharacterAttribute Health;
	Health.CurrentValue = AttributeMap[EAttributeType::Health].CurrentValue;
	Health.MinValue = AttributeMap[EAttributeType::Health].MinValue;
	Health.MaxValue = AttributeMap[EAttributeType::Health].MaxValue;
	HealthArray.Add(Health);

	FCharacterAttribute Mana;
	Mana.CurrentValue = AttributeMap[EAttributeType::Mana].CurrentValue;
	Mana.MinValue = AttributeMap[EAttributeType::Mana].MinValue;
	Mana.MaxValue = AttributeMap[EAttributeType::Mana].MaxValue;
	ManaArray.Add(Mana);

	FCharacterAttribute Stamina;
	Stamina.CurrentValue = AttributeMap[EAttributeType::Stamina].CurrentValue;
	Stamina.MinValue = AttributeMap[EAttributeType::Stamina].MinValue;
	Stamina.MaxValue = AttributeMap[EAttributeType::Stamina].MaxValue;
	StaminaArray.Add(Stamina);

	FCharacterAttribute Experience;
	Experience.CurrentValue = AttributeMap[EAttributeType::Experience].CurrentValue;
	Experience.MinValue = AttributeMap[EAttributeType::Experience].MinValue;
	Experience.MaxValue = AttributeMap[EAttributeType::Experience].MaxValue;
	ExperienceArray.Add(Experience);

	FCharacterAttribute Level;
	Level.CurrentValue = AttributeMap[EAttributeType::Level].CurrentValue;
	Level.MinValue = AttributeMap[EAttributeType::Level].MinValue;
	Level.MaxValue = AttributeMap[EAttributeType::Level].MaxValue;
	LevelArray.Add(Level);

	FCharacterAttribute Money;
	Money.CurrentValue = AttributeMap[EAttributeType::Money].CurrentValue;
	Money.MinValue = AttributeMap[EAttributeType::Money].MinValue;
	Money.MaxValue = AttributeMap[EAttributeType::Money].MaxValue;
	MoneyArray.Add(Money);
}

void URPGSaveGame::LoadPlayerAttribute(TMap<EAttributeType,FAttribute>& AttributeMap, int Index) const
{
	if(Index >= HealthArray.Num())
		return;
	
	AttributeMap[EAttributeType::Health].CurrentValue = HealthArray[Index].CurrentValue;
	AttributeMap[EAttributeType::Health].MinValue = HealthArray[Index].MinValue;
	AttributeMap[EAttributeType::Health].MaxValue = HealthArray[Index].MaxValue;

	AttributeMap[EAttributeType::Mana].CurrentValue = ManaArray[Index].CurrentValue;
	AttributeMap[EAttributeType::Mana].MinValue = ManaArray[Index].MinValue;
	AttributeMap[EAttributeType::Mana].MaxValue = ManaArray[Index].MaxValue;

	AttributeMap[EAttributeType::Stamina].CurrentValue = StaminaArray[Index].CurrentValue;
	AttributeMap[EAttributeType::Stamina].MinValue = StaminaArray[Index].MinValue;
	AttributeMap[EAttributeType::Stamina].MaxValue = StaminaArray[Index].MaxValue;

	AttributeMap[EAttributeType::Experience].CurrentValue = ExperienceArray[Index].CurrentValue;
	AttributeMap[EAttributeType::Experience].MinValue = ExperienceArray[Index].MinValue;
	AttributeMap[EAttributeType::Experience].MaxValue = ExperienceArray[Index].MaxValue;

	AttributeMap[EAttributeType::Level].CurrentValue = LevelArray[Index].CurrentValue;
	AttributeMap[EAttributeType::Level].MinValue = LevelArray[Index].MinValue;
	AttributeMap[EAttributeType::Level].MaxValue = LevelArray[Index].MaxValue;

	AttributeMap[EAttributeType::Money].CurrentValue = MoneyArray[Index].CurrentValue;
	AttributeMap[EAttributeType::Money].MinValue = MoneyArray[Index].MinValue;
	AttributeMap[EAttributeType::Money].MaxValue = MoneyArray[Index].MaxValue;
}

void URPGSaveGame::SavePlayerAbility(const TArray<TSubclassOf<AAbilityBase>>& PlayerAbilityList)
{
	FSaveAbilityList AbilityList;
	AbilityList.PlayerAbilityList = PlayerAbilityList;
	PlayerAbilityListArray.Add(AbilityList);
}

TArray<TSubclassOf<AAbilityBase>> URPGSaveGame::GetPlayerAbilityList(int Index) const
{
	if(Index >= PlayerAbilityListArray.Num())
		return TArray<TSubclassOf<AAbilityBase>>();
	return PlayerAbilityListArray[Index].PlayerAbilityList;
}

void URPGSaveGame::SavePlayerQuest(const TArray<class AMasterQuest*>& AssignedQuestList,const TArray<class AMasterQuest*>& CompleteQuestList)
{
	FSaveQuestList QuestList;
	for (AMasterQuest* Quest : AssignedQuestList)
	{
		QuestList.AssignedQuestList.Add(Quest->GetClass());
		FAssignedQuestInfo AssignedQuestInfo;
		AssignedQuestInfo.CurrentSubGoalTargetNumList = Quest->GetCurrentSubGoalTargetNumList();
		AssignedQuestInfo.CurrentSubGoalInQuestIDList = Quest->GetCurrentSubGoalIDList();
		AssignedQuestInfo.CompleteSubGoalInfoList = Quest->GetCompleteSubGoalInfoList();
		AssignedQuestInfo.CurrentSubGoalInfoList = Quest->GetCurrentSubGoalInfoList();
		QuestList.AssignedQuestInfoList.Add(AssignedQuestInfo);
	}
	
	for (const AMasterQuest* Quest : CompleteQuestList)
	{
		QuestList.CompleteQuestList.Add(Quest->GetClass());
	}
	QuestListArray.Add(QuestList);
}

FSaveQuestList URPGSaveGame::GetSavedQuestList(int Index) const
{
	if(Index >= QuestListArray.Num())
		return FSaveQuestList();
	return QuestListArray[Index];
}

