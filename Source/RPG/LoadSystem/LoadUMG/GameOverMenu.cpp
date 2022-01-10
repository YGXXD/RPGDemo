// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverMenu.h"

#include "Components/Button.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"

bool UGameOverMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	BackMainMenu = Cast<UButton>(GetWidgetFromName("BackMainMenu"));
	if(BackMainMenu)
	{
		BackMainMenu->OnClicked.AddDynamic(this,&UGameOverMenu::OnBackMainMenuClick);
	}
	return true;
}

void UGameOverMenu::OnBackMainMenuClick()
{
	OPEN_LEVEL(GetWorld(),TEXT("StartMap"));
}
