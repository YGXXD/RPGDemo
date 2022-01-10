// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadListItem.h"

#include "LoadMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"

bool ULoadListItem::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	LoadIndex = -1;
	
	ReadLoadButton = Cast<UButton>(GetWidgetFromName("ReadLoadButton"));
	if(ReadLoadButton)
	{
		ReadLoadButton->OnClicked.AddDynamic(this,&ULoadListItem::OnReadLoadButtonClick);
	}
	
	DeleteButton = Cast<UButton>(GetWidgetFromName("DeleteButton"));
	if(DeleteButton)
	{
		DeleteButton->OnClicked.AddDynamic(this,&ULoadListItem::OnDeleteButtonClick);
	}
	
	SaveTimeText = Cast<UTextBlock>(GetWidgetFromName("SaveTimeText"));

	return true;
}

void ULoadListItem::SetLoadMenu(ULoadMenu* const Menu)
{
	LoadMenu = Menu;
}

void ULoadListItem::OnReadLoadButtonClick()
{
	SAVE_MANAGER->SetLoadIndex(LoadIndex);
	OPEN_LEVEL(GetWorld(),TEXT("GamePlayMap"));
}

void ULoadListItem::OnDeleteButtonClick()
{
	SAVE_MANAGER->DeleteSaveDataByIndex(LoadIndex);
	if(LoadMenu)
	{
		LoadMenu->UpdateLoadMenu();
	}
}

void ULoadListItem::UpdateLoadListItem(int Index, FString TimeString)
{
	LoadIndex = Index;
	if(SaveTimeText)
	{
		SaveTimeText->SetText(FText::FromString(TimeString));
	}
}

