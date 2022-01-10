// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "DashInAir.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ADashInAir : public AAbilityBase
{
	GENERATED_BODY()
private:
	virtual void AbilityStart() override;
	virtual void AbilityEnd() override;
public:
	ADashInAir(const FObjectInitializer& ObjectInitializer);
};
