// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPatrolPathPoint.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UFindPatrolPathPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	UFindPatrolPathPoint(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
