// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UGameOverMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UButton* BackMainMenu;

	virtual bool Initialize() override;
	UFUNCTION()
	void OnBackMainMenuClick();
};
