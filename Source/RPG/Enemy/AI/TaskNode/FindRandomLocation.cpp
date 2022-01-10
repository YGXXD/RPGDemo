// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "RPG/Enemy/AI/BlackBoardKeys.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"

UFindRandomLocation::UFindRandomLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Find Random Location");
	SearchRadius = 200.0f;
}

EBTNodeResult::Type UFindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	const AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	const APawn* Enemy = AIController->GetPawn();
	const FVector Origin = Enemy->GetOwner()->GetActorLocation();//获取生成器的位置
	FNavLocation TargetLocation;
	const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if(NavigationSystem->GetRandomPointInNavigableRadius(Origin,SearchRadius,TargetLocation,nullptr))
	{
		AIController->GetBlackboard()->SetValueAsVector(EnemyBlackBoardKey::TargetLocation,TargetLocation.Location);
	}

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

