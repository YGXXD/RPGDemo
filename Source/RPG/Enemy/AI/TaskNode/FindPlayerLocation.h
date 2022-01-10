// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UFindPlayerLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	bool bSearchRandom;

	UPROPERTY(EditAnywhere)
	float SearchRadius;
	
	UFindPlayerLocation(const FObjectInitializer& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
