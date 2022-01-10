// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "GamePlayLevel.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AGamePlayLevel : public ALevelScriptActor
{
	GENERATED_BODY()
private:
	FTimerHandle DelayLoadHandle;
	
	UPROPERTY()
	class UUserWidget* Loading;
protected:
	virtual void BeginPlay() override;
};
