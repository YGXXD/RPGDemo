// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "IncrementPathIndex.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UIncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	enum class EDirectionType
	{
		Forward,
		Reverse
	};
	EDirectionType DirectionType;
	
	UIncrementPathIndex(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
