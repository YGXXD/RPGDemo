// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/SceneCapture2D.h"
#include "MiniMapCapture.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API AMiniMapCapture : public ASceneCapture2D
{
	GENERATED_BODY()
private:
	float MaxFOVAngleValue;
	float MinFOVAngleValue;
		
	UPROPERTY()
	class USceneCaptureComponent2D* CaptureComp;
public:
	AMiniMapCapture(const FObjectInitializer& ObjectInitializer);

	USceneCaptureComponent2D* GetMiniCapture() const;
	void ModifyMiniCaptureFOVAngle(float Value) const;
};
