// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AEnemyController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(VisibleAnywhere)
	class UBehaviorTreeComponent* BehaviorTreeComp;

	UPROPERTY(VisibleAnywhere)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionStimuliSourceComponent* StimuliComp;

	UPROPERTY()
	class UBehaviorTree* Tree;

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;
	
	virtual void BeginPlay() override;
	void SetUpPerceptionSystem();
	
	UFUNCTION()
	void OnSightUpdate(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	void OnHealthUpdate(float HealthPercent);
public:
	AEnemyController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void OnPossess(APawn* InPawn) override;
	UBlackboardComponent* GetBlackboard() const;
};
