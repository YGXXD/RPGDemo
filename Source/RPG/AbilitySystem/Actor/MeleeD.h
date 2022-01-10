// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "MeleeD.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMeleeD : public AAbilityBase
{
	GENERATED_BODY()
protected:
	virtual void AbilityStart() override;
public:
	AMeleeD(const FObjectInitializer& ObjectInitializer);
	virtual void AbilityVfxEffect(IAbilityAttackInterface* DamageInterface) override;
};
