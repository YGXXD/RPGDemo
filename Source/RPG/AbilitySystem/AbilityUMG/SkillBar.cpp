// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBar.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "RPG/AbilitySystem/Actor/AbilityBase.h"

bool USkillBar::Initialize()
{
	if(!Super::Initialize())
	{
		return false;
	}
	
	SkillImageID = 0;
	
	UImage* SkillImage1 = Cast<UImage>(GetWidgetFromName("SkillImage1"));
	UTextBlock* SkillCDText1 = Cast<UTextBlock>(GetWidgetFromName("SkillCDText1"));
	if(SkillImage1 && SkillCDText1)
	{
		SkillImageList.Add(SkillImage1);
		SkillCDTextList.Add(SkillCDText1);
	}
	
	UImage* SkillImage2 = Cast<UImage>(GetWidgetFromName("SkillImage2"));
	UTextBlock* SkillCDText2 = Cast<UTextBlock>(GetWidgetFromName("SkillCDText2"));
	if(SkillImage2 && SkillCDText2)
	{
		SkillImageList.Add(SkillImage2);
		SkillCDTextList.Add(SkillCDText2);
	}
	
	UImage* SkillImage3 = Cast<UImage>(GetWidgetFromName("SkillImage3"));
	UTextBlock* SkillCDText3 = Cast<UTextBlock>(GetWidgetFromName("SkillCDText3"));
	if(SkillImage3 && SkillCDText3)
	{
		SkillImageList.Add(SkillImage3);
		SkillCDTextList.Add(SkillCDText3);
	}
	
	UImage* SkillImage4 = Cast<UImage>(GetWidgetFromName("SkillImage4"));
	UTextBlock* SkillCDText4 = Cast<UTextBlock>(GetWidgetFromName("SkillCDText4"));
	if(SkillImage4 && SkillCDText4)
	{
		SkillImageList.Add(SkillImage4);
		SkillCDTextList.Add(SkillCDText4);
	}
	
	UImage* SkillImage5 = Cast<UImage>(GetWidgetFromName("SkillImage5"));
	UTextBlock* SkillCDText5 = Cast<UTextBlock>(GetWidgetFromName("SkillCDText5"));
	if(SkillImage5)
	{
		SkillImageList.Add(SkillImage5);
		SkillCDTextList.Add(SkillCDText5);
	}

	InitializeSkillBar();

	return true;
}

void USkillBar::InitializeSkillBar()
{
	for (int i = 0; i < SkillImageList.Num(); i++)
	{
		SkillImageList[i]->SetColorAndOpacity(FLinearColor(1,1,1,0));
		SkillImageList[i]->SetBrushFromMaterial(nullptr);
		SkillImageNameMap.Add(SkillImageList[i],"");
	}
	for (int i = 0; i < SkillCDTextList.Num(); i++)
	{
		SkillCDTextList[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void USkillBar::AddSkillToSkillBar(AAbilityBase* Skill)
{
	if(Skill->GetAbilityType() == EAbilityType::Skill)
	{
		if(Skill->GetUMGIconMaterialInstance())
		{
			SkillImageList[SkillImageID]->SetBrushFromMaterial(Skill->GetUMGIconMaterialInstance());
			SkillImageList[SkillImageID]->SetColorAndOpacity(FLinearColor(1,1,1));
			
			Skill->SetSkillImageDynamic(SkillImageList[SkillImageID]->GetDynamicMaterial());
			Skill->SetSkillCDText(SkillCDTextList[SkillImageID]);
			
			SkillImageNameMap[SkillImageList[SkillImageID]] = Skill->GetAbilityName();
			SkillImageID++;
			SkillImageID %= 5;
		}
	}
}

FString USkillBar::GetSkillNameInImageList(int ImageListID) const
{
	if(ImageListID < SkillImageList.Num() && ImageListID >= 0)
	{
		return SkillImageNameMap[SkillImageList[ImageListID]];
	}
	return "";
}
