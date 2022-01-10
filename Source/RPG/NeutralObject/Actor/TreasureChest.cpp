// Fill out your copyright notice in the Description page of Project Settings.


#include "TreasureChest.h"

#include "Components/WidgetComponent.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "RPG/NeutralObject/InteractionUMG/NeutralObjectInteraction.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/QuestSystem/Component/QuestComponent.h"

ATreasureChest::ATreasureChest(const FObjectInitializer& ObjectInitializer) : ATargetObject(ObjectInitializer)
{
	//初始化属性
	Name = TEXT("藏宝箱");
	bIsOpen = false;
		
	//初始化宝箱盖子Mesh
	TreasureChestLidComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TreasureChestLid"));
	TreasureChestLidComp->SetupAttachment(RootComponent);
	TreasureChestLidComp->SetRelativeLocation(FVector(-35,0,50));
	TreasureChestLidComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//初始化金币Mesh
	CoinComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Coin"));
	CoinComp->SetupAttachment(RootComponent);
	CoinComp->SetRelativeLocation(FVector(0,0,30));
	CoinComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	//初始化默认类
	static ConstructorHelpers::FObjectFinder<UStaticMesh> TreasureChestMesh(TEXT("StaticMesh'/Game/RPG_Game/StaticMesh/SM_Chest_Bottom.SM_Chest_Bottom'"));
	if(TreasureChestMesh.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(TreasureChestMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> TreasureChestLidMesh(TEXT("StaticMesh'/Game/RPG_Game/StaticMesh/SM_Chest_Lid.SM_Chest_Lid'"));
	if(TreasureChestLidMesh.Succeeded())
	{
		TreasureChestLidComp->SetStaticMesh(TreasureChestLidMesh.Object);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CoinMesh(TEXT("StaticMesh'/Game/RPG_Game/StaticMesh/SM_Pickup_Coin.SM_Pickup_Coin'"));
	if(CoinMesh.Succeeded())
	{
		CoinComp->SetStaticMesh(CoinMesh.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UPaperSprite> MiniMapCharacterSprite(TEXT("PaperSprite'/Game/RPG_Game/Textures/MiniMapNPC_Sprite.MiniMapNPC_Sprite'"));
	if(MiniMapCharacterSprite.Succeeded())
	{
		PaperSpriteComp->SetSprite(MiniMapCharacterSprite.Object);
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat>  CF_OpenTreasure(TEXT("CurveFloat'/Game/RPG_Game/TimelineCurve/CF_OpenTreasure.CF_OpenTreasure'"));
	if(CF_OpenTreasure.Succeeded())
	{
		OpenTreasureCurveFloat = CF_OpenTreasure.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> CF_CoinRotator(TEXT("CurveFloat'/Game/RPG_Game/TimelineCurve/CF_CoinRotator.CF_CoinRotator'"));
	if(CF_CoinRotator.Succeeded())
	{
		CoinRotatorCurveFloat = CF_CoinRotator.Object;
	}
}

void ATreasureChest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	OpenTreasureTimeline.TickTimeline(DeltaSeconds);
	CoinRotatorTimeline.TickTimeline(DeltaSeconds);
}

void ATreasureChest::InitializeTargetObject()
{
	Super::InitializeTargetObject();

	//绑定打开宝箱的时间线
	FOnTimelineFloatStatic OpenTreasureCallBack;
	auto OpenTreasureLambda = [this](float Value)
	{
		const FRotator EndRotation(110,0,0);
		TreasureChestLidComp->SetRelativeRotation(FMath::Lerp<FRotator>(FRotator::ZeroRotator,EndRotation,Value));
		const FVector EndVector(0,0,85);
		CoinComp->SetRelativeLocation(FMath::Lerp<FVector>(FVector(0,0,30),EndVector,Value));
	};
	OpenTreasureCallBack.BindLambda(OpenTreasureLambda);
	OpenTreasureTimeline.AddInterpFloat(OpenTreasureCurveFloat,OpenTreasureCallBack);
	//绑定硬币旋转时间线
	FOnTimelineFloatStatic CoinRotatorCallBack;
	auto CoinRotatorLambda = [this](float Value)
	{
		const FRotator EndRotation(0,1080,0);
		CoinComp->SetRelativeRotation(FMath::Lerp<FRotator>(FRotator::ZeroRotator,EndRotation,Value));
	};
	CoinRotatorCallBack.BindLambda(CoinRotatorLambda);
	CoinRotatorTimeline.AddInterpFloat(CoinRotatorCurveFloat,CoinRotatorCallBack);

	FOnTimelineEvent OpenFinishCallBack;
	OpenFinishCallBack.BindDynamic(this,&ATreasureChest::CoinRotatorStart);
	OpenTreasureTimeline.SetTimelineFinishedFunc(OpenFinishCallBack);
	
	FOnTimelineEvent CoinRotatorFinishCallBack;
	CoinRotatorFinishCallBack.BindDynamic(this,&ATreasureChest::DestroyCoinComp);
	CoinRotatorTimeline.SetTimelineFinishedFunc(CoinRotatorFinishCallBack);
}

#pragma region 硬币的旋转和摧毁

void ATreasureChest::CoinRotatorStart()
{
	CoinRotatorTimeline.PlayFromStart();
}

void ATreasureChest::DestroyCoinComp()
{
	CoinComp->DestroyComponent();
}

#pragma endregion 

#pragma region 玩家交互接口实现

void ATreasureChest::InteractWithTargetObject()
{
	Super::InteractWithTargetObject();
	if(!bIsOpen)
	{
		Player->GetQuestComponent()->OnSubGoalTargetComplete(GetClass());
		OpenTreasureTimeline.PlayFromStart();
		PaperSpriteComp->DestroyComponent();
		bIsOpen = true;
	}
}

#pragma endregion 
