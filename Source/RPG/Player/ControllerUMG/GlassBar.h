// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GlassBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UGlassBar : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere)
	FLinearColor DefaultColor;
	
	UPROPERTY()
	class UImage* GlassBarImage;

	UPROPERTY()
	class UMaterialInstanceDynamic* MaterialInstance;

	virtual bool Initialize() override;
	void InitGlassBarColor() const;

public:
	void UpdateGlassBarValue(float CurrentValue, float MaxValue) const;

	void UpdateGlassBarColor(float CurrentValue, float MaxValue, const FLinearColor& ToColor) const;
};
