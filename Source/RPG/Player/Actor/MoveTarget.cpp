// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveTarget.h"

#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "RPG/Player/Controller/RPGamePlayerController.h"


// Sets default values
AMoveTarget::AMoveTarget(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = SceneComp;
	
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(SceneComp);
	BoxComp->SetBoxExtent(FVector(75,40,40));
	BoxComp->SetRelativeRotation(FRotator(90,0,0));
	BoxComp->SetCollisionObjectType(ECC_WorldDynamic);
	
	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("TargetDecal"));
	DecalComp->SetupAttachment(SceneComp);
	DecalComp->DecalSize = FVector(70,35,35);
	DecalComp->SetRelativeRotation(FRotator(90,0,0));
}

void AMoveTarget::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&AMoveTarget::BoxBeginOverlap);
}

void AMoveTarget::BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARPGamePlayerController* Controller = Cast<ARPGamePlayerController>(GetOwner());
	if(Controller)
	{
		if(Controller->GetPawn() == OtherActor)
		{
			//被玩家碰到之后，触发CancelMouseMove，他的激活属性会变为false
			Controller->CancelMouseMove();
		}
	}
}

void AMoveTarget::SetActorActive(bool bIsActive)
{
	SetActorHiddenInGame(!bIsActive);
	SetActorTickEnabled(bIsActive);
	SetActorEnableCollision(bIsActive);
}

