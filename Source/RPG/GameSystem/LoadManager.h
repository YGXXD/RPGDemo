// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoadManager.generated.h"

#define SAVE_MANAGER ULoadManager::GetInstance()
#define SAVE_DATA ULoadManager::GetInstance()->GetSaveGameInstance()
#define DESTROY_SAVE_MANAGER ULoadManager::DestroyLoadManager()
/**
 * 
 */
UCLASS()
class RPG_API ULoadManager : public UObject
{
	GENERATED_BODY()

private:
	static class ULoadManager* LoadManagerInstance;
	int LoadIndex;
	
	UPROPERTY()
	class URPGSaveGame* SaveGameInstance;
	ULoadManager(const FObjectInitializer& ObjectInitializer);

public:
	static ULoadManager* GetInstance();
	static void DestroyLoadManager();
	
	URPGSaveGame* GetSaveGameInstance() const;
	void SetLoadIndex(int Index);
	
	void SaveGame(UWorld* World);
	void LoadGame(UWorld* World);
	void DeleteSaveDataByIndex(int Index);
};
