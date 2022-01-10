// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadMenu.h"

#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/GameSystem/WidgetPool.h"
#include "RPG/LoadSystem/SaveData/RPGSaveGame.h"
#include "RPG/LoadSystem/LoadUMG/LoadListItem.h"

bool ULoadMenu::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	LoadScrollBox = Cast<UScrollBox>(GetWidgetFromName("LoadScrollBox"));

	BackMenuButton = Cast<UButton>(GetWidgetFromName("BackMenuButton"));
	if(BackMenuButton)
	{
		BackMenuButton->OnClicked.AddDynamic(this,&ULoadMenu::OnBackMenuButtonClick);
	}
	
	MainMenuClass = LoadClass<UMainMenu>(this,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Menu/WBP_MainMenu.WBP_MainMenu_C'"));
	
	LoadListItemClass =
		LoadClass<ULoadListItem>(this,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Menu/WBP_LoadListItem.WBP_LoadListItem_C'"));
	if(LoadListItemClass)
	{
		CREATE_WIDGET_POOL(LoadListItemClass);
	}
	return true;
}

void ULoadMenu::OnBackMenuButtonClick()
{
	RemoveFromViewport();
	UMainMenu* MainMenu = CreateWidget<UMainMenu>(GetWorld(),MainMenuClass);
	if(MainMenu)
	{
		MainMenu->AddToViewport();
	}
}

void ULoadMenu::NativeConstruct()
{
	Super::NativeConstruct();
	UpdateLoadMenu();
}

void ULoadMenu::UpdateLoadMenu()
{
	const TArray<FString> TimeArray = SAVE_DATA->GetTimeArray();
	WIDGET_POOL->RemoveWidgetAllChildren<ULoadListItem>(LoadListItemClass,LoadScrollBox);
	for (int i = TimeArray.Num() - 1; i >= 0; i--)
	{
		ULoadListItem* LoadListItem = WIDGET_POOL->GetUserWidgetFromUMGPool<ULoadListItem>(LoadListItemClass,GetWorld());
		if(LoadListItem)
		{
			LoadListItem->SetLoadMenu(this);
			LoadListItem->UpdateLoadListItem(i,TimeArray[i]);
			LoadScrollBox->AddChild(LoadListItem);
		}
	}
}
