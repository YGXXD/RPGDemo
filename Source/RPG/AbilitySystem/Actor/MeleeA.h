// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "MeleeA.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMeleeA : public AAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void AbilityStart() override;
public:
	AMeleeA(const FObjectInitializer& ObjectInitializer);
	virtual void AbilityVfxEffect(IAbilityAttackInterface* DamageInterface) override;
};
