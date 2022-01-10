// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "PlayerInMeleeRange.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UPlayerInMeleeRange : public UBTService_BlackboardBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float MeleeRange;
	
	UPlayerInMeleeRange();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
