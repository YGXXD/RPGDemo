// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillBar.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API USkillBar : public UUserWidget
{
	GENERATED_BODY()
private:
	UPROPERTY()
	TMap<class UImage*, FString> SkillImageNameMap;

	UPROPERTY()
	TArray<class UImage*> SkillImageList;

	UPROPERTY()
	TArray<class UTextBlock*> SkillCDTextList;

	int SkillImageID;
	
	virtual bool Initialize() override;;
	void InitializeSkillBar();
	
public:
	FString GetSkillNameInImageList(int ImageListID) const;
	void AddSkillToSkillBar(class AAbilityBase* Skill);
};
