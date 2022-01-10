// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPath.h"

APatrolPath::APatrolPath(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

}

FVector APatrolPath::GetPatrolPoint(int Index) const
{
	return PatrolPointList[Index];
}

int APatrolPath::GetPatrolPointNum() const
{
	return PatrolPointList.Num();
}
