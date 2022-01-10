// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityAttackInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAbilityAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API IAbilityAttackInterface
{
	GENERATED_BODY()

public:
	virtual void TakeDamageForCharacter(float Value) = 0;
	virtual void TakeDamageWithEffect() = 0;
	virtual void CharacterToDie() = 0;
};
