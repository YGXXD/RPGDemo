// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "RPG/LoadSystem/LoadUMG/LoadMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"


bool UMainMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	NewGame = Cast<UButton>(GetWidgetFromName("NewGame"));
	if(NewGame)
	{
		NewGame->OnClicked.AddDynamic(this,&UMainMenu::OnStartGameClick);
	}
	
	ContinueGame = Cast<UButton>(GetWidgetFromName("ContinueGame"));
	if(ContinueGame)
	{
		ContinueGame->OnClicked.AddDynamic(this,&UMainMenu::OnContinueGameClick);
	}

	QuitGame = Cast<UButton>(GetWidgetFromName("QuitGame"));
	if(QuitGame)
	{
		QuitGame->OnClicked.AddDynamic(this,&UMainMenu::OnQuitGameClick);
	}

	LoadMenuClass =
		LoadClass<ULoadMenu>(this,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Menu/WBP_LoadMenu.WBP_LoadMenu_C'"));

	return true;
}

void UMainMenu::OnStartGameClick()
{
	SAVE_MANAGER->SetLoadIndex(-1);
	OPEN_LEVEL(GetWorld(),TEXT("GamePlayMap"));
}

void UMainMenu::OnContinueGameClick()
{
	RemoveFromViewport();
	ULoadMenu* LoadMenu = CreateWidget<ULoadMenu>(GetWorld(),LoadMenuClass);
	if(LoadMenu)
	{
		LoadMenu->AddToViewport();
	}
}

void UMainMenu::OnQuitGameClick()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("QUIT");
}
