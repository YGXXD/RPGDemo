// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPG/NeutralObject/Actor/TargetObject.h"
#include "SavePoint.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ASavePoint : public ATargetObject
{
	GENERATED_BODY()
private:
	//绑定玩家交互的接口
	virtual void InteractWithTargetObject() override;

public:
	ASavePoint(const FObjectInitializer& ObjectInitializer);
};
