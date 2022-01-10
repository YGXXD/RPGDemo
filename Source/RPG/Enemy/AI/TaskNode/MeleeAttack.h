// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMeleeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	UMeleeAttack(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	bool IsMeleeFinish(const class AEnemyBase* Enemy) const;
};
