// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TSubclassOf<class ULoadMenu> LoadMenuClass;
	
	UPROPERTY()
	class UButton* NewGame;

	UPROPERTY()
	class UButton* ContinueGame;

	UPROPERTY()
	class UButton* QuitGame;
	
	virtual bool Initialize() override;
	
	UFUNCTION()
	void OnStartGameClick();
	
	UFUNCTION()
	void OnContinueGameClick();

	UFUNCTION()
	void OnQuitGameClick();
};
