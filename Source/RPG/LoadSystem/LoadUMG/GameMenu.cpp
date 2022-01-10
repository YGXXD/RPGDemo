// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMenu.h"

#include "Components/Button.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"
#include "RPG/Player/ControllerUMG/PlayerControllerMainUI.h"

bool UGameMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	SetVisibility(ESlateVisibility::Collapsed);
	
	ContinueButton = Cast<UButton>(GetWidgetFromName("ContinueButton"));
	if(ContinueButton)
	{
		ContinueButton->OnClicked.AddDynamic(this,&UGameMenu::OnContinueButtonClick);
	}
	OptionButton = Cast<UButton>(GetWidgetFromName("OptionButton"));
	BackMainMenuButton = Cast<UButton>(GetWidgetFromName("BackMainMenuButton"));
	if(BackMainMenuButton)
	{
		BackMainMenuButton->OnClicked.AddDynamic(this,&UGameMenu::OnBackMainMenuButtonClick);
	}
	
	return true;
}

void UGameMenu::OnContinueButtonClick()
{
	if(PCMainUI)
	{
		PCMainUI->SwitchGameMenu();
	}
}

void UGameMenu::OnBackMainMenuButtonClick()
{
	OPEN_LEVEL(GetWorld(),TEXT("StartMap"));
}

void UGameMenu::SetPCMainUI(UPlayerControllerMainUI* MainUI)
{
	PCMainUI = MainUI;
}
