// Fill out your copyright notice in the Description page of Project Settings.


#include "SavePoint.h"

#include "PaperSpriteComponent.h"
#include "Components/WidgetComponent.h"
#include "RPG/GameSystem/LoadManager.h"

ASavePoint::ASavePoint(const FObjectInitializer& ObjectInitializer) : ATargetObject(ObjectInitializer)
{
	Name = TEXT("存档点");
	
	PaperSpriteComp->SetRelativeLocation(FVector(0,0,150));
	static  ConstructorHelpers::FObjectFinder<UPaperSprite> SavePointMiniMapIcon(TEXT("PaperSprite'/Game/RPG_Game/Textures/MiniMapSavePoint_Sprite.MiniMapSavePoint_Sprite'"));
	if(SavePointMiniMapIcon.Succeeded())
	{
		PaperSpriteComp->SetSprite(SavePointMiniMapIcon.Object);
	}
	
	static  ConstructorHelpers::FObjectFinder<UStaticMesh> SM_SavePoint(TEXT("StaticMesh'/Game/RPG_Game/StaticMesh/SM_SavePoint.SM_SavePoint'"));
	if(SM_SavePoint.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(SM_SavePoint.Object);
	}
}

void ASavePoint::InteractWithTargetObject()
{
	Super::InteractWithTargetObject();
	SAVE_MANAGER->SaveGame(GetWorld());
}
