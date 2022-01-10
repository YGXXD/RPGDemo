// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "NumberText.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UNumberText : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	class UTextBlock* NumberTextBlock;

	virtual bool Initialize() override;

public:
	void SetNumberTextValue(EAttributeType Key,float ModifyValue);
};
