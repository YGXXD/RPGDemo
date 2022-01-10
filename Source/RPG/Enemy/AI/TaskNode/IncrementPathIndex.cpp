// Fill out your copyright notice in the Description page of Project Settings.


#include "IncrementPathIndex.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "RPG/Enemy/AI/BlackBoardKeys.h"
#include "RPG/Enemy/AI/Actor/PatrolPath.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"
#include "RPG/Enemy/Character/EnemyBase.h"

UIncrementPathIndex::UIncrementPathIndex(const FObjectInitializer& ObjectInitializer)
{
	DirectionType = EDirectionType::Forward;
	NodeName = TEXT("Increment Path Index");
}

EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	const AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	const AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	if(Enemy->GetPatrolPath())
	{
		const int PathPointNum = Enemy->GetPatrolPath()->GetPatrolPointNum();
		int Index = AIController->GetBlackboard()->GetValueAsInt(EnemyBlackBoardKey::PatrolPathIndex);
		if(Index >= PathPointNum-1 && DirectionType == EDirectionType::Forward)
		{
			DirectionType = EDirectionType::Reverse;
		}
		else if(Index <= 0 && DirectionType == EDirectionType::Reverse)
		{
			DirectionType = EDirectionType::Forward;
		}
		AIController->GetBlackboard()->SetValueAsInt(EnemyBlackBoardKey::PatrolPathIndex, DirectionType == EDirectionType::Forward? ++Index : --Index);
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
