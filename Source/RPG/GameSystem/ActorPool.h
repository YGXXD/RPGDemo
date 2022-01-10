// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorPool.generated.h"

#define ACTOR_POOL UActorPool::GetInstance()
#define CREATE_ACTOR_POOL UActorPool::GetInstance()->AddActorToPoolMap
#define DELETE_ACTOR_POOL UActorPool::GetInstance()->RemoveActorFromPoolMap
#define EMPTY_ACTOR_POOL UActorPool::GetInstance()->EmptyActorPool()
#define DESTROY_ACTOR_POOL UActorPool::DestroyActorPool()
/**
 * 
 */
USTRUCT()
struct FActorPoolItem
{
	GENERATED_BODY()
	UPROPERTY()
	TArray<AActor*> Pool;
};

UCLASS()
class RPG_API UActorPool : public UObject
{
	GENERATED_BODY()
private:
	static class UActorPool* ActorPoolInstance;
	
	UPROPERTY()
	TMap<TSubclassOf<class AActor>, FActorPoolItem> ActorPoolMap;
	
	UActorPool(const FObjectInitializer& ObjectInitializer);

public:
	static UActorPool* GetInstance();
	static void DestroyActorPool();
	
	void AddActorToPoolMap(TSubclassOf<class AActor> ActorClass);
	void RemoveActorFromPoolMap(TSubclassOf<class AActor> ActorClass);
	void EmptyActorPool();
	
	void DeleteActor(AActor* const Actor);
	
	template <typename ActorT = AActor>
	ActorT* GetActorFromActorPool(TSubclassOf<class AActor> ActorClass, UWorld* World);

	template <typename ActorT = AActor>
	ActorT* SetActorFromActorPool(TSubclassOf<class AActor> ActorClass, UWorld* World, const FTransform SpawnTransform);

	template <typename ActorT = AActor>
	ActorT* SetActorFromActorPool(TSubclassOf<class AActor> ActorClass, UWorld* World, const FVector Location = FVector::ZeroVector, const FRotator Rotation = FRotator::ZeroRotator, const FVector Scale = FVector::OneVector);
};
