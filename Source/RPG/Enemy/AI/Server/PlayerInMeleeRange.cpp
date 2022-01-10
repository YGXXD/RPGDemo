// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInMeleeRange.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Enemy/AI/BlackBoardKeys.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"
#include "RPG/Enemy/Character/EnemyBase.h"

UPlayerInMeleeRange::UPlayerInMeleeRange()
{
	MeleeRange = 150;
	NodeName = TEXT("Player In Melee Range");
}

void UPlayerInMeleeRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());
	const AEnemyBase* Enemy = Cast<AEnemyBase>(AIController->GetPawn());
	
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	
	AIController->GetBlackboard()->SetValueAsBool(EnemyBlackBoardKey::IsPlayerInMeleeRange,Enemy->GetDistanceTo(Player) <= MeleeRange);

}