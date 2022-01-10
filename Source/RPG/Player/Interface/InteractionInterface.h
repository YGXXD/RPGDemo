// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
	
};

/**
 * 
 */
class RPG_API IInteractionInterface
{
	GENERATED_BODY()
	
public:
	virtual void BeginOverlapTargetObject() = 0;
	virtual void EndOverlapTargetObject() = 0;
	virtual void InteractWithTargetObject() = 0;
};
