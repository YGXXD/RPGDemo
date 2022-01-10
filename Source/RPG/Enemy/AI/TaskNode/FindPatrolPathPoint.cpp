// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPatrolPathPoint.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "RPG/Enemy/AI/BlackBoardKeys.h"
#include "RPG/Enemy/AI/Actor/PatrolPath.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"
#include "RPG/Enemy/Character/EnemyBase.h"

UFindPatrolPathPoint::UFindPatrolPathPoint(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Find Patrol Path Point");
}

EBTNodeResult::Type UFindPatrolPathPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	const AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	const AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	if(Enemy->GetPatrolPath())
	{
		const int Index = AIController->GetBlackboard()->GetValueAsInt(EnemyBlackBoardKey::PatrolPathIndex);
		const FVector PatrolPoint = Enemy->GetPatrolPath()->GetPatrolPoint(Index);//获取的是相对PatrolActor的位置
		const FVector PatrolTargetPoint = Enemy->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPoint);//获取绝对位置
		AIController->GetBlackboard()->SetValueAsVector(EnemyBlackBoardKey::PatrolPathPoint,PatrolTargetPoint);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
