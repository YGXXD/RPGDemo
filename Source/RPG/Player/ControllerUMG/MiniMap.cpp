// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"
#include "RPG/Player/Actor/MiniMapCapture.h"


bool UMiniMap::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}

	bIsShowDistance = false;
	
	TargetDirectionImage = Cast<UImage>(GetWidgetFromName("TargetDirectionImage"));
	if(TargetDirectionImage)
	{
		TargetDirectionImage->SetVisibility(ESlateVisibility::Collapsed);
	}
	
	DistanceText = Cast<UTextBlock>(GetWidgetFromName("DistanceText"));
	
	UOverlay* DistanceOverlay = Cast<UOverlay>(GetWidgetFromName("DistanceOverlay"));
	if(DistanceOverlay)
	{
		DistanceOverlay->SetRenderScale(FVector2D(0,1));
	}
	
	//获取动画
	UWidgetBlueprintGeneratedClass* WidgetClass = GetWidgetTreeOwningClass();
	if(WidgetClass && WidgetClass->Animations.Num())
	{
		ShowTargetDistanceAnimation = WidgetClass->Animations[0];
	}
	
	PlusMapButton = Cast<UButton>(GetWidgetFromName("PlusMapButton"));
	if(PlusMapButton)
	{
		PlusMapButton->OnClicked.AddDynamic(this,&UMiniMap::PlusMapSize);
	}
	
	ReduceMapButton = Cast<UButton>(GetWidgetFromName("ReduceMapButton"));
	if(ReduceMapButton)
	{
		ReduceMapButton->OnClicked.AddDynamic(this,&UMiniMap::ReduceMapSize);
	}
	
	return true;
}

void UMiniMap::PlusMapSize()
{
	if(MiniMapCapture)
	{
		MiniMapCapture->ModifyMiniCaptureFOVAngle(-10);
	}
}

void UMiniMap::ReduceMapSize()
{
	if(MiniMapCapture)
	{
		MiniMapCapture->ModifyMiniCaptureFOVAngle(10);
	}
}

void UMiniMap::SetMiniMapCapture(class AMiniMapCapture* const Capture)
{
	this->MiniMapCapture = Capture;
}

void UMiniMap::SetDistance(float Value)
{
	if(DistanceText)
	{
		const FString Distance = FString::FromInt(Value) + TEXT("米");
		DistanceText->SetText(FText::FromString(Distance));
	}
}

void UMiniMap::SetDirection(float Value)
{
	if(TargetDirectionImage)
	{
		TargetDirectionImage->SetRenderTransformAngle(Value);
	}
}

UImage* UMiniMap::GetTargetDirectionImage() const
{
	return TargetDirectionImage;
}

void UMiniMap::ShowTargetDistanceOverlay()
{
	if(!bIsShowDistance)
	{
		PlayAnimation(ShowTargetDistanceAnimation);
		bIsShowDistance = true;
		TargetDirectionImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMiniMap::HideTargetDistanceOverlay()
{
	if(bIsShowDistance)
	{
		PlayAnimation(ShowTargetDistanceAnimation,0,1,EUMGSequencePlayMode::Reverse,1);
		bIsShowDistance = false;
		TargetDirectionImage->SetVisibility(ESlateVisibility::Collapsed);
	}
}
