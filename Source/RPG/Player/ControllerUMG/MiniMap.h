// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiniMap.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UMiniMap : public UUserWidget
{
	GENERATED_BODY()
private:
	bool bIsShowDistance;
	
	UPROPERTY()
	class UButton* PlusMapButton;

	UPROPERTY()
	class UButton* ReduceMapButton;

	UPROPERTY()
	class AMiniMapCapture* MiniMapCapture;

	UPROPERTY()
	class UTextBlock* DistanceText;

	UPROPERTY()
	class UImage* TargetDirectionImage;

	UPROPERTY()
	class UWidgetAnimation* ShowTargetDistanceAnimation;

	virtual bool Initialize() override;
	
	UFUNCTION()
	void PlusMapSize();

	UFUNCTION()
	void ReduceMapSize();

public:
	void SetMiniMapCapture(class AMiniMapCapture* const Capture);
	void SetDistance(float Value);
	void SetDirection(float Value);
	UImage* GetTargetDirectionImage() const;
	void ShowTargetDistanceOverlay();
	void HideTargetDistanceOverlay();
};
