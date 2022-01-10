// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadManager.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/LoadSystem/SaveData/RPGSaveGame.h"
#include "RPG/Player/Interface/SaveLoadInterface.h"


ULoadManager* ULoadManager::LoadManagerInstance = nullptr;

ULoadManager::ULoadManager(const FObjectInitializer& ObjectInitializer) : UObject(ObjectInitializer)
{
	LoadIndex = -1;
	if (UGameplayStatics::DoesSaveGameExist("MySaveGame", 0))
	{
		SaveGameInstance = Cast<URPGSaveGame>(UGameplayStatics::LoadGameFromSlot("MySaveGame", 0));
	}
	else
	{
		SaveGameInstance = Cast<URPGSaveGame>(UGameplayStatics::CreateSaveGameObject(URPGSaveGame::StaticClass()));
	}
}

ULoadManager* ULoadManager::GetInstance()
{
	if(!LoadManagerInstance)
	{
		LoadManagerInstance = NewObject<ULoadManager>();
		LoadManagerInstance->AddToRoot();
	}
	return LoadManagerInstance;
}

void ULoadManager::DestroyLoadManager()
{
	if(LoadManagerInstance)
	{
		LoadManagerInstance->RemoveFromRoot();
		LoadManagerInstance = nullptr;
	}
}

URPGSaveGame* ULoadManager::GetSaveGameInstance() const
{
	return SaveGameInstance;
}

void ULoadManager::SetLoadIndex(int Index)
{
	LoadIndex = Index;
}

void ULoadManager::SaveGame(UWorld* World)
{
	if(SaveGameInstance)
	{
		for (TActorIterator<ACharacter> ActorItr(World); ActorItr; ++ActorItr)
		{
			if(Cast<ISaveLoadInterface>(*ActorItr))
			{
				Cast<ISaveLoadInterface>(*ActorItr)->SaveInfo();
			}
		}
		const FDateTime DateTime = UKismetMathLibrary::Now();
		const FString Time = FString::FromInt(DateTime.GetYear()) + TEXT("年") +
			 FString::FromInt(DateTime.GetMonth()) + TEXT("月") +
			 	 FString::FromInt(DateTime.GetDay()) + TEXT("日 ") +
			 	 	FString::FromInt(DateTime.GetHour()) + TEXT(":") +
			 	 		FString::FromInt(DateTime.GetMinute()) + TEXT(":") +
			 	 			FString::FromInt(DateTime.GetSecond());
		SaveGameInstance->SaveTimeString(Time);
		if(UGameplayStatics::SaveGameToSlot(SaveGameInstance,"MySaveGame",0))
		{
			UKismetSystemLibrary::PrintString(World,TEXT("游戏已保存"));
			UE_LOG(LogTemp,Warning,TEXT("保存成功"));
		}
	}
}

void ULoadManager::LoadGame(UWorld* World)
{
	if(LoadIndex == -1)
	{
		return;
	}
	if(SaveGameInstance)
	{
		for (TActorIterator<ACharacter> ActorItr(World); ActorItr; ++ActorItr)
		{
			if(Cast<ISaveLoadInterface>(*ActorItr))
			{
				Cast<ISaveLoadInterface>(*ActorItr)->LoadInfo(LoadIndex);
			}
		}
	}
}

void ULoadManager::DeleteSaveDataByIndex(int Index)
{
	SaveGameInstance->DeleteDataByIndex(Index);
	if(UGameplayStatics::SaveGameToSlot(SaveGameInstance,"MySaveGame",0))
		UE_LOG(LogTemp,Warning,TEXT("删除成功"));
}
