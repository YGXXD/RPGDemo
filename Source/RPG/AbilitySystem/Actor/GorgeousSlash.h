// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "GorgeousSlash.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AGorgeousSlash : public AAbilityBase
{
	GENERATED_BODY()
protected:
	virtual void AbilityStart() override;
public:
	AGorgeousSlash(const FObjectInitializer& ObjectInitializer);
};
