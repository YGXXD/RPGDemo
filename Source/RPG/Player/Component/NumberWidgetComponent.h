// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "NumberWidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UNumberWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
	UFUNCTION()
	void DelayDestroy();
public:
	UNumberWidgetComponent();
	void AddNumberTextToView(EAttributeType Key,float Value);
};
