// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TargetObject.h"
#include "Components/TimelineComponent.h"
#include "TreasureChest.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API ATreasureChest : public ATargetObject
{
	GENERATED_BODY()

private:
	bool bIsOpen;//宝藏箱是否打开
	FTimeline OpenTreasureTimeline;
	FTimeline CoinRotatorTimeline;
	
	UPROPERTY()
	class UCurveFloat* OpenTreasureCurveFloat;

	UPROPERTY()
	class UCurveFloat* CoinRotatorCurveFloat;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* TreasureChestLidComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* CoinComp;//金币

	//硬币旋转
	UFUNCTION()
	void CoinRotatorStart();
	//摧毁硬币
	UFUNCTION()
	void DestroyCoinComp();
protected:
	virtual void Tick(float DeltaSeconds) override;
	
	//在父类的BeginPlay函数中会调用
	virtual void InitializeTargetObject() override;
	
	//绑定玩家交互的接口
	virtual void InteractWithTargetObject() override;
public:
	ATreasureChest(const FObjectInitializer& ObjectInitializer);

};
