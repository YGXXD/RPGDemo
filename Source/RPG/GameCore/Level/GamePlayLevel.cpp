// Fill out your copyright notice in the Description page of Project Settings.


#include "GamePlayLevel.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/GameSystem/LoadManager.h"

void AGamePlayLevel::BeginPlay()
{
	Super::BeginPlay();
	const TSubclassOf<UUserWidget> LoadingClass = LoadClass<UUserWidget>(nullptr,TEXT("WidgetBlueprint'/Game/RPG_Game/UI/Menu/WBP_Loading.WBP_Loading_C'"));
	if(LoadingClass)
	{
		Loading = CreateWidget(GetWorld(),LoadingClass);
		Loading->AddToViewport(1);
	}
	
	auto LoadGameLambda = [this]()
	{
		//得用延时加载
		SAVE_MANAGER->LoadGame(GetWorld());
		if(Loading)
		{
			Loading->RemoveFromViewport();
			Loading = nullptr;
		}
	};
	const FTimerDelegate LoadGameDelegate = FTimerDelegate::CreateLambda(LoadGameLambda);
	GetWorldTimerManager().SetTimer(DelayLoadHandle,LoadGameDelegate,1,false);
}
