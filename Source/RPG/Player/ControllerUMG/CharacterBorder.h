// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "CharacterBorder.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UCharacterBorder : public UUserWidget
{
	GENERATED_BODY()

private:
	float Money;
	float TargetMoney;
	FTimerHandle MoneyUpdateHandle;
	
	UPROPERTY()
	class UStateBar* HealthBar;

	UPROPERTY()
	class UStateBar* ManaBar;

	UPROPERTY()
	class UProgressBar* StaminaBar;

	UPROPERTY()
	class UTextBlock* LevelText;

	UPROPERTY()
	class UTextBlock* MoneyNumText;

	virtual bool Initialize() override;
	void UpdateHealthBar(float CurrentValue, float MaxValue);
	void UpdateManaBar(float CurrentValue, float MaxValue);
	void UpdateStaminaBar(float CurrentValue, float MaxValue);
	void UpdateLevelText(float CurrentValue);
	void UpdateMoneyNumText(float CurrentValue);
	
public:
	void UpdateCharacterBorder(EAttributeType Key, float CurrentValue, float MaxValue);
};
