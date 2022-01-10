// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "WildStabbing.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AWildStabbing : public AAbilityBase
{
	GENERATED_BODY()
protected:
	virtual void AbilityStart() override;
public:
	AWildStabbing(const FObjectInitializer& ObjectInitializer);
};
