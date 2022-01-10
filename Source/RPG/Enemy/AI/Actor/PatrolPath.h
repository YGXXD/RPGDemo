// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

UCLASS()
class RPG_API APatrolPath : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere,Category="AI",meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
	TArray<FVector> PatrolPointList;
	
public:	
	APatrolPath(const FObjectInitializer& ObjectInitializer);
	FVector GetPatrolPoint(int Index) const;
	int GetPatrolPointNum() const;

};
