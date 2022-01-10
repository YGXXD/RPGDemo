// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetObject.h"

#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "RPG/NeutralObject/InteractionUMG/NeutralObjectInteraction.h"
#include "RPG/Player/Character/RPGameCharacter.h"

ATargetObject::ATargetObject(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	//初始化StaticMesh组件
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	RootComponent = StaticMeshComp;

	//初始化WidgetComp组件
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(StaticMeshComp);
	WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	WidgetComp->SetHiddenInGame(true);
	static ConstructorHelpers::FClassFinder<UNeutralObjectInteraction> WBP_NeutralObjectInteraction(TEXT("/Game/RPG_Game/UI/Talk/WBP_NeutralObjectInteraction"));
	if(WBP_NeutralObjectInteraction.Succeeded())
	{
		WidgetComp->SetWidgetClass(WBP_NeutralObjectInteraction.Class);
	}

	//初始化PaperSprite组件
	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	PaperSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PaperSpriteComp->SetupAttachment(StaticMeshComp);
	PaperSpriteComp->SetSpriteColor(FLinearColor(0,0,1));
	PaperSpriteComp->SetRelativeLocation(FVector(0,0,140));
	PaperSpriteComp->SetRelativeRotation(FRotator(0,90,-90));
	PaperSpriteComp->SetRelativeScale3D(FVector(0.5,0.5,0.5));
	PaperSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PaperSpriteComp->SetOwnerNoSee(true);

}

void ATargetObject::BeginPlay()
{
	Super::BeginPlay();
	InitializeTargetObject();
}

void ATargetObject::InitializeTargetObject()
{
	Player = Cast<ARPGameCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if(Player)
	{
		SetOwner(Player);
	}
	UNeutralObjectInteraction* NameWidget = Cast<UNeutralObjectInteraction>(WidgetComp->GetUserWidgetObject());
	NameWidget->SetNameText(Name);
}

#pragma region 玩家交互接口实现

void ATargetObject::BeginOverlapTargetObject()
{
	WidgetComp->SetHiddenInGame(false);
	Player->OnInteract.AddDynamic(this,&ATargetObject::InteractWithTargetObject);
}

void ATargetObject::EndOverlapTargetObject()
{
	WidgetComp->SetHiddenInGame(true);
	Player->OnInteract.RemoveDynamic(this,&ATargetObject::InteractWithTargetObject);
}

void ATargetObject::InteractWithTargetObject()
{
	
}

#pragma endregion 
