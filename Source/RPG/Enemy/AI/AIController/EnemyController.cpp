// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "RPG/Enemy/AI/BlackBoardKeys.h"
#include "RPG/Enemy/Character/EnemyBase.h"
#include "RPG/Player/Character/RPGameCharacter.h"

AEnemyController::AEnemyController(const FObjectInitializer& ObjectInitializer)
{
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	StimuliComp = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("StimuliComp"));
	StimuliComp->RegisterForSense(TSubclassOf<UAISense_Sight>());
	StimuliComp->RegisterWithPerceptionSystem();

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT_EnemyTree(TEXT("BehaviorTree'/Game/RPG_Game/Blueprints/Enemy/AI/BT_EnemyTree.BT_EnemyTree'"));
	if(BT_EnemyTree.Succeeded())
	{
		Tree = BT_EnemyTree.Object;
	}

	SetUpPerceptionSystem();
}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();
	RunBehaviorTree(Tree);
	BehaviorTreeComp->StartTree(*Tree);
}

void AEnemyController::OnHealthUpdate(float HealthPercent)
{
	if(HealthPercent <= 0.3)
	{
		BlackboardComp->SetValueAsBool(EnemyBlackBoardKey::LowHealth,true);
	}
	UE_LOG(LogTemp,Warning,TEXT("bool = %d"),BlackboardComp->GetValueAsBool(EnemyBlackBoardKey::LowHealth) ? 1:0);
}

void AEnemyController::SetUpPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp")));
	SightConfig->SightRadius = 500;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 50;
	SightConfig->PeripheralVisionAngleDegrees = 90;
	SightConfig->SetMaxAge(5);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(SightConfig->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfig);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this,&AEnemyController::OnSightUpdate);
}

void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if(AEnemyBase* Enemy = Cast<AEnemyBase>(InPawn))
	{
		Enemy->HealthUpdate.AddDynamic(this,&AEnemyController::OnHealthUpdate);
	}
	if(BlackboardComp)
	{
		BlackboardComp->InitializeBlackboard(*Tree->BlackboardAsset);
	}
}

void AEnemyController::OnSightUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if(Cast<ARPGameCharacter>(Actor))
	{
		BlackboardComp->SetValueAsBool(EnemyBlackBoardKey::CanSeePlayer,Stimulus.WasSuccessfullySensed());
	}
}

UBlackboardComponent* AEnemyController::GetBlackboard() const
{
	return BlackboardComp;
}
