// Fill out your copyright notice in the Description page of Project Settings.


#include "ChangeSpeed.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"
#include "RPG/Enemy/Character/EnemyBase.h"

UChangeSpeed::UChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	const AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	const AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	Enemy->GetEnemyMovementComponent()->MaxWalkSpeed = Speed;
}

FString UChangeSpeed::GetStaticServiceDescription() const
{
	const FString SeverSystemInfo = Super::GetStaticServiceDescription();
	return SeverSystemInfo + "Enemy Speed Was Changed";
}
