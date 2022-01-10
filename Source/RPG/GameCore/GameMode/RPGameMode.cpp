// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameMode.h"

#include "EngineUtils.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/Player/Controller/RPGamePlayerController.h"


ARPGameMode::ARPGameMode()
{
	static ConstructorHelpers::FClassFinder<ARPGameCharacter> BP_RPGameCharacter(TEXT("/Game/RPG_Game/Blueprints/Player/BP_RPGameCharacter"));
	if(BP_RPGameCharacter.Succeeded())
	{
		DefaultPawnClass = BP_RPGameCharacter.Class;
	}

	static ConstructorHelpers::FClassFinder<ARPGamePlayerController> BP_RPGamePlayerController(TEXT("/Game/RPG_Game/Blueprints/Player/BP_RPGamePlayerController"));
	if(BP_RPGamePlayerController.Succeeded())
	{
		PlayerControllerClass = BP_RPGamePlayerController.Class;
	}
	
}