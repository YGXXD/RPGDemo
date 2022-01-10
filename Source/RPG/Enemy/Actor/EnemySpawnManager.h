// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnManager.generated.h"

UCLASS()
class RPG_API AEnemySpawnManager : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly,Category="Enemy")
	int MaxEnemyNum;

	UPROPERTY(EditDefaultsOnly,Category="Enemy")
	TSubclassOf<class AEnemyBase> EnemyClass;
	
	FTimerHandle SpawnEnemyHandle;

	UPROPERTY()
	TArray<class AEnemyBase*> EnemyList;

protected:
	virtual void BeginPlay() override;

public:
	AEnemySpawnManager(const FObjectInitializer& ObjectInitializer);
	virtual void Tick(float DeltaTime) override;
	void RemoveEnemy(AEnemyBase* Enemy);

};
