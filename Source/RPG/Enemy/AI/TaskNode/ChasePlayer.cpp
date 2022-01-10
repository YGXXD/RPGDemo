// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "RPG/Enemy/AI/BlackBoardKeys.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"

UChasePlayer::UChasePlayer(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	const FVector PlayerLocation = AIController->GetBlackboard()->GetValueAsVector(EnemyBlackBoardKey::TargetLocation);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController,PlayerLocation);
	
	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
