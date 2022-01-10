// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameInstance.h"

#include "RPG/GameSystem/ActorPool.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/GameSystem/WidgetPool.h"

URPGameInstance::URPGameInstance(const FObjectInitializer& ObjectInitializer) : UGameInstance(ObjectInitializer)
{
	
}

void URPGameInstance::Init()
{
	Super::Init();
	WIDGET_POOL;
	ACTOR_POOL;
	UE_LOG(LogTemp,Warning,TEXT("sadsad"))
	SAVE_MANAGER;
}

void URPGameInstance::Shutdown()
{
	Super::Shutdown();
	DESTROY_WIDGET_POOL;
	DESTROY_ACTOR_POOL;
	DESTROY_SAVE_MANAGER;
}
