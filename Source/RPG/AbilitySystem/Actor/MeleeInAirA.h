// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "MeleeInAirA.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMeleeInAirA : public AAbilityBase
{
	GENERATED_BODY()
protected:
	virtual void AbilityStart() override;
	virtual void AbilityEnd() override;
public:
	AMeleeInAirA(const FObjectInitializer& ObjectInitializer);
	virtual void AbilityVfxEffect(IAbilityAttackInterface* DamageInterface) override;
};
