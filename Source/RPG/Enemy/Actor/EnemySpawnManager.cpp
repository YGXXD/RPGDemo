// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnManager.h"

#include "RPG/Enemy/Character/EnemyBase.h"
#include "RPG/GameSystem/ActorPool.h"

AEnemySpawnManager::AEnemySpawnManager(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	MaxEnemyNum = 5;
}

void AEnemySpawnManager::BeginPlay()
{
	Super::BeginPlay();
	CREATE_ACTOR_POOL(EnemyClass);
	
	for (int i = 0; i < MaxEnemyNum; i++)
	{
		AEnemyBase* Enemy = ACTOR_POOL->SetActorFromActorPool<AEnemyBase>(EnemyClass,GetWorld(),
			GetActorLocation()+FVector(FMath::RandHelper(200),FMath::RandHelper(200),FMath::RandHelper(20)));
		Enemy->SetOwner(this);
		Enemy->InitializeEnemy();
		EnemyList.Add(Enemy);
	}
}

void AEnemySpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawnManager::RemoveEnemy(AEnemyBase* Enemy)
{
	ACTOR_POOL->DeleteActor(Enemy);
	EnemyList.Remove(Enemy);
	if(!EnemyList.Num())
	{
		auto EnemySpawnLambda = [this]()
		{
			for (int i = 0; i < MaxEnemyNum; i++)
			{
				AEnemyBase* Enemy = ACTOR_POOL->SetActorFromActorPool<AEnemyBase>(EnemyClass,GetWorld(),
					GetActorLocation()+FVector(FMath::RandHelper(200),FMath::RandHelper(200),FMath::RandHelper(20)));
				Enemy->InitializeEnemy();
				EnemyList.Add(Enemy);
			}
		};
		const FTimerDelegate EnemySpawnDelegate = FTimerDelegate::CreateLambda(EnemySpawnLambda);
		GetWorldTimerManager().SetTimer(SpawnEnemyHandle,EnemySpawnDelegate,10,false);
	}
}
