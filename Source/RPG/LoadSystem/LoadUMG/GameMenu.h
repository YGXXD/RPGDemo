// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UGameMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UPlayerControllerMainUI* PCMainUI;
	
	UPROPERTY()
	class UButton* ContinueButton;

	UPROPERTY()
	class UButton* OptionButton;

	UPROPERTY()
	class UButton* BackMainMenuButton;

	virtual bool Initialize() override;
	UFUNCTION()
	void OnContinueButtonClick();

	UFUNCTION()
	void OnBackMainMenuButtonClick();

public:
	void SetPCMainUI(class UPlayerControllerMainUI* PCMainUI);
};
