// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorPool.h"


UActorPool* UActorPool::ActorPoolInstance = nullptr;

UActorPool::UActorPool(const FObjectInitializer& ObjectInitializer) : UObject(ObjectInitializer)
{
	
}

UActorPool* UActorPool::GetInstance()
{
	if(!ActorPoolInstance)
	{
		ActorPoolInstance = NewObject<UActorPool>();
		ActorPoolInstance->AddToRoot();
	}
	return ActorPoolInstance;
}

void UActorPool::DestroyActorPool()
{
	if(ActorPoolInstance)
	{
		ActorPoolInstance->RemoveFromRoot();
		ActorPoolInstance = nullptr;
	}
}

void UActorPool::AddActorToPoolMap(TSubclassOf<AActor> ActorClass)
{
	if(ActorClass && !ActorPoolMap.Contains(ActorClass))
	{
		const FActorPoolItem ActorPool;
		ActorPoolMap.Add(ActorClass,ActorPool);
	}
}

void UActorPool::RemoveActorFromPoolMap(TSubclassOf<AActor> ActorClass)
{
	if(ActorClass && ActorPoolMap.Contains(ActorClass))
	{
		ActorPoolMap.Remove(ActorClass);
	}
}

void UActorPool::EmptyActorPool()
{
	ActorPoolMap.Empty();
}

void UActorPool::DeleteActor(AActor* const Actor)
{
	if(ActorPoolMap.Contains(Actor->GetClass()))
	{
		Actor->SetActorTickEnabled(false);
		Actor->SetActorEnableCollision(false);
		Actor->SetActorHiddenInGame(true);
		ActorPoolMap[Actor->GetClass()].Pool.Add(Actor);
	}
}

template <typename ActorT>
ActorT* UActorPool::GetActorFromActorPool(TSubclassOf<class AActor> ActorClass, UWorld* World)
{
	ActorT* Actor = nullptr;
	if(ActorPoolMap.Contains(ActorClass))
	{
		if(ActorPoolMap[ActorClass].Pool.Num())
		{
			Actor = Cast<ActorT>(ActorPoolMap[ActorClass].Pool[0]);
			ActorPoolMap[ActorClass].Pool.RemoveAt(0);
			Actor->SetActorTickEnabled(true);
			Actor->SetActorEnableCollision(true);
			Actor->SetActorHiddenInGame(false);
			UE_LOG(LogTemp,Warning,TEXT("调用对象池"));
		}
		else
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			Actor = World->SpawnActor<ActorT>(ActorClass,SpawnParams);
			UE_LOG(LogTemp,Warning,TEXT("生成Actor"));
		}
	}

	return Actor;
}

template <typename ActorT>
ActorT* UActorPool::SetActorFromActorPool(TSubclassOf<AActor> ActorClass, UWorld* World, const FTransform SpawnTransform)
{
	ActorT* Actor = GetActorFromActorPool<ActorT>(ActorClass, World);
	
	Actor->SetActorTransform(SpawnTransform);
	
	return Actor;
}

template <typename ActorT>
ActorT* UActorPool::SetActorFromActorPool(TSubclassOf<AActor> ActorClass, UWorld* World, const FVector Location, const FRotator Rotation, const FVector Scale)
{
	ActorT* Actor = GetActorFromActorPool<ActorT>(ActorClass, World);
	
	Actor->SetActorLocation(Location);
	Actor->SetActorRotation(Rotation);
	Actor->SetActorScale3D(Scale);
	
	return Actor;
}
