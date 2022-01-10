// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapCapture.h"

#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

AMiniMapCapture::AMiniMapCapture(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	MaxFOVAngleValue = 150;
	MinFOVAngleValue = 50;
	
	CaptureComp = GetCaptureComponent2D();
	static ConstructorHelpers::FObjectFinder<UTextureRenderTarget2D> RT_MiniMapTexture(TEXT("TextureRenderTarget2D'/Game/RPG_Game/Materials/RT_MiniMap.RT_MiniMap'"));
	if(RT_MiniMapTexture.Succeeded())
	{
		CaptureComp->TextureTarget = RT_MiniMapTexture.Object;
	}
	
}

USceneCaptureComponent2D* AMiniMapCapture::GetMiniCapture() const
{
	return CaptureComp;
}

void AMiniMapCapture::ModifyMiniCaptureFOVAngle(float Value) const
{
	CaptureComp->FOVAngle = FMath::Clamp(CaptureComp->FOVAngle + Value, MinFOVAngleValue, MaxFOVAngleValue);
}
