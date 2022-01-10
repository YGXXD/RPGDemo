// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityBase.h"
#include "HealthRegen.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AHealthRegen : public AAbilityBase
{
	GENERATED_BODY()
public:
	AHealthRegen(const FObjectInitializer& ObjectInitializer);
	virtual void AbilityTick(float DeltaTickTime) override;
};
