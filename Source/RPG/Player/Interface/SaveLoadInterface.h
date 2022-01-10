// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveLoadInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveLoadInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RPG_API ISaveLoadInterface
{
	GENERATED_BODY()

public:

	virtual void SaveInfo() = 0;
	virtual void LoadInfo(int Index) = 0;
};
