// Fill out your copyright notice in the Description page of Project Settings.


#include "GlassBar.h"

#include "Components/Image.h"
#include "Kismet/KismetMathLibrary.h"

bool UGlassBar::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	GlassBarImage = Cast<UImage>(GetWidgetFromName("GlassBarImage"));
	if(GlassBarImage)
	{
		MaterialInstance = GlassBarImage->GetDynamicMaterial();
		InitGlassBarColor();
	}

	return true;
}

void UGlassBar::InitGlassBarColor() const
{
	MaterialInstance->SetVectorParameterValue(TEXT("Color"),DefaultColor);
}

void UGlassBar::UpdateGlassBarValue(float CurrentValue, float MaxValue) const
{
	const float Value = FMath::Clamp<float>(CurrentValue/MaxValue,0,1);
	MaterialInstance->SetScalarParameterValue(TEXT("Percentage"),Value);
}

void UGlassBar::UpdateGlassBarColor(float CurrentValue, float MaxValue, const FLinearColor& ToColor) const
{
	const float Value = FMath::Clamp<float>(CurrentValue/MaxValue,0,1);
	const FLinearColor Color = UKismetMathLibrary::LinearColorLerpUsingHSV(ToColor,DefaultColor,Value);
	MaterialInstance->SetVectorParameterValue(TEXT("Color"),Color);
}
