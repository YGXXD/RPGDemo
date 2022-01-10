// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "RPGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	URPGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init() override;
	virtual void Shutdown() override;
};
