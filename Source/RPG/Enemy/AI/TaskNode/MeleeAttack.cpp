// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttack.h"

#include "RPG/Enemy/AI/AIController/EnemyController.h"
#include "RPG/Enemy/Character/EnemyBase.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"

UMeleeAttack::UMeleeAttack(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Melee Attack");
}

EBTNodeResult::Type UMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	const AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	if(IsMeleeFinish(Enemy))
	{
		Enemy->MeleeAttack();
	}

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UMeleeAttack::IsMeleeFinish(const AEnemyBase* Enemy) const
{
	return URPGSystemLibrary::IsAnimMontageStop(Enemy->GetMesh()->GetAnimInstance(),Enemy->GetMeleeAttackMontage());
}
