// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "ChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	float Speed = 300;
	
	UChangeSpeed();
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual FString GetStaticServiceDescription() const override;
};
