// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "BuffBlue.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ABuffBlue : public AEnemyBase
{
	GENERATED_BODY()

protected:
	virtual void CharacterToDie() override;
	
public:
	ABuffBlue(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void InitializeEnemy() override;
	
};
