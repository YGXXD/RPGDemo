// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadMenu.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ULoadMenu : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TSubclassOf<class UMainMenu> MainMenuClass;
	
	UPROPERTY()
	TSubclassOf<class ULoadListItem> LoadListItemClass;
	
	UPROPERTY()
	class UScrollBox* LoadScrollBox;

	UPROPERTY()
	class UButton* BackMenuButton;
	
	virtual bool Initialize() override;
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnBackMenuButtonClick();
	
public:
	void UpdateLoadMenu();
};
