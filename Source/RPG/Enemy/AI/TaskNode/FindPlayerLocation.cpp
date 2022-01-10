// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Enemy/AI/BlackBoardKeys.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"

UFindPlayerLocation::UFindPlayerLocation(const FObjectInitializer& ObjectInitializer)
{
	NodeName = TEXT("Find Player Location");
	bSearchRandom = false;
	SearchRadius = 200;
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
	const AEnemyController* AIController = Cast<AEnemyController>(OwnerComp.GetAIOwner());

	const FVector PlayerLocation = Player->GetActorLocation();
	if (bSearchRandom)
	{
		FNavLocation TargetLocation;
		const UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetCurrent(GetWorld());
		if(NavigationSystem->GetRandomPointInNavigableRadius(PlayerLocation,SearchRadius,TargetLocation,nullptr))
		{
			AIController->GetBlackboard()->SetValueAsVector(EnemyBlackBoardKey::TargetLocation,TargetLocation.Location);
		}
	}
	else
	{
		AIController->GetBlackboard()->SetValueAsVector(EnemyBlackBoardKey::TargetLocation,PlayerLocation);
	}

	FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}
