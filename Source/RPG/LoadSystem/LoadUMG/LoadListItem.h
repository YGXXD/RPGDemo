// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadListItem.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ULoadListItem : public UUserWidget
{
	GENERATED_BODY()

private:
	int LoadIndex;

	UPROPERTY()
	class ULoadMenu* LoadMenu;
	
	UPROPERTY()
	class UButton* ReadLoadButton;

	UPROPERTY()
	class UButton* DeleteButton;

	UPROPERTY()
	class UTextBlock* SaveTimeText;
	
	virtual bool Initialize() override;

	UFUNCTION()
	void OnReadLoadButtonClick();

	UFUNCTION()
	void OnDeleteButtonClick();

public:
	void SetLoadMenu(class ULoadMenu* const Menu);
	void UpdateLoadListItem(int Index, FString TimeString);
};
