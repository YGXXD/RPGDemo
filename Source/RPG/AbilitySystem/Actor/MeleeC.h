// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "MeleeC.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMeleeC : public AAbilityBase
{
	GENERATED_BODY()

protected:
	virtual void AbilityStart() override;
public:
	AMeleeC(const FObjectInitializer& ObjectInitializer);
	virtual void AbilityVfxEffect(IAbilityAttackInterface* DamageInterface) override;
};
