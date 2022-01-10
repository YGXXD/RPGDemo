// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/Enemy/Actor/EnemySpawnManager.h"
#include "RPG/Enemy/AI/AIController/EnemyController.h"
#include "RPG/Enemy/EnemyUMG/EnemyStateBar.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/QuestSystem/Component/QuestComponent.h"

AEnemyBase::AEnemyBase(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;
	ExpValue = 30;
	AutoPossessAI = EAutoPossessAI::Spawned;
	
	EnemyMesh = GetMesh();
	EnemyMovementComp = GetCharacterMovement();
	EnemyMovementComp->bUseControllerDesiredRotation = true;
	EnemyMovementComp->bOrientRotationToMovement = true;

	StateWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("StateWidgetComp"));
	StateWidgetComp->SetupAttachment(GetCapsuleComponent());
	StateWidgetComp->SetDrawSize(FVector2D(200,20));
	StateWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	static ConstructorHelpers::FClassFinder<UEnemyStateBar> WBP_EnemyStateBar(TEXT("/Game/RPG_Game/UI/Enemy/WBP_EnemyStateBar"));
	if(WBP_EnemyStateBar.Succeeded())
	{
		StateWidgetComp->SetWidgetClass(WBP_EnemyStateBar.Class);
	}

	ResetHealth(100);

	static ConstructorHelpers::FClassFinder<AEnemyController> BP_EnemyController(TEXT("/Game/RPG_Game/Blueprints/Enemy/AI/BP_EnemyController"));
	if(BP_EnemyController.Succeeded())
	{
		AIControllerClass = BP_EnemyController.Class;
	}
}

void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
}

UCharacterMovementComponent* AEnemyBase::GetEnemyMovementComponent() const
{
	return EnemyMovementComp;
}

APatrolPath* AEnemyBase::GetPatrolPath() const
{
	return PatrolPath;
}

UAnimMontage* AEnemyBase::GetMeleeAttackMontage() const
{
	return MeleeAttackMontage;
}

void AEnemyBase::ResetHealth(float CurrentValue, float MaxValue, float MinValue)
{
	if(CurrentValue > MaxValue)
	{
		Health.MaxValue = CurrentValue;
	}
	else
	{
		Health.MaxValue = MaxValue;
	}
	Health.CurrentValue = CurrentValue;
	Health.MinValue = MinValue;
}

void AEnemyBase::InitializeEnemy()
{
	PLAY_MONTAGE(GetMesh()->GetAnimInstance(),SpawnMontage);
	UAIBlueprintHelperLibrary::GetAIController(this);
	ResetHealth(100);
	if(UEnemyStateBar* StateBar = Cast<UEnemyStateBar>(StateWidgetComp->GetWidget()))
	{
		StateBar->SetEnemyStateBar(Health.CurrentValue,Health.MaxValue);
	}
	StateWidgetComp->SetHiddenInGame(true);
}

void AEnemyBase::MeleeAttack()
{
	PLAY_MONTAGE(GetMesh()->GetAnimInstance(),MeleeAttackMontage);
}

void AEnemyBase::TakeDamageForCharacter(float Value)
{
	if(Value <= 0 || Health.CurrentValue <= Health.MinValue)
	{
		return;
	}
	Value = FMath::Clamp<float>(Value,0,Health.CurrentValue);
	Health.CurrentValue -= Value;
	HealthUpdate.Broadcast(Health.CurrentValue/Health.MaxValue);
	if(UEnemyStateBar* StateBar = Cast<UEnemyStateBar>(StateWidgetComp->GetWidget()))
	{
		StateBar->UpdateEnemyStateBar(Health.CurrentValue,Health.MaxValue);
	}
	HealthUpdate.Broadcast(Health.CurrentValue/Health.MaxValue);
	if(Health.CurrentValue <= Health.MinValue)
	{
		CharacterToDie();
	}
}

void AEnemyBase::TakeDamageWithEffect()
{	StateWidgetComp->SetHiddenInGame(false);
	URPGSystemLibrary::StopAllMontage(GetMesh()->GetAnimInstance(),0.25);
	PLAY_MONTAGE(GetMesh()->GetAnimInstance(),HitMontage);
	GetWorldTimerManager().PauseTimer(HideStateHandle);
	auto HideStateLambda = [this]()
	{
		StateWidgetComp->SetHiddenInGame(true);
	};
	const FTimerDelegate HideStateDelegate = FTimerDelegate::CreateLambda(HideStateLambda);
	GetWorldTimerManager().SetTimer(HideStateHandle,HideStateDelegate,5,false);
}

void AEnemyBase::CharacterToDie()
{
	ARPGameCharacter* Player = Cast<ARPGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if(Player)
	{
		Player->GetQuestComponent()->OnSubGoalTargetComplete(GetClass());
		Player->ModifyAttribute(EAttributeType::Experience,ExpValue);
	}
	const float DelayTime = PLAY_MONTAGE(GetMesh()->GetAnimInstance(),DeathMontage);
	DelayDeleteEnemy(DelayTime);
	
}

void AEnemyBase::DelayDeleteEnemy(float DelayTime)
{
	auto DelayDeleteEnemyLambda = [this]()
	{
		AEnemySpawnManager* EnemySpawnManager = Cast<AEnemySpawnManager>(GetOwner());
		if(EnemySpawnManager)
		{
			EnemySpawnManager->RemoveEnemy(this);
		}
	};
	
	const FTimerDelegate DelayDeleteEnemyDelegate = FTimerDelegate::CreateLambda(DelayDeleteEnemyLambda);
	GetWorldTimerManager().SetTimer(DelayDeleteHandle,DelayDeleteEnemyDelegate,DelayTime,false);
}
