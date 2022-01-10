// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RPG/Player/Interface/InteractionInterface.h"
#include "TargetObject.generated.h"

UCLASS()
class RPG_API ATargetObject : public AActor, public IInteractionInterface 
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere,Category="NActor")
	FString Name;

	UPROPERTY()
	class ARPGameCharacter* Player;
	
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* WidgetComp;
	
	//小地图图标组件
	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* PaperSpriteComp;
	
	virtual void BeginPlay() override;
	virtual void InitializeTargetObject();

	//绑定玩家交互的接口
	UFUNCTION()
	virtual void InteractWithTargetObject() override;
public:	
	ATargetObject(const FObjectInitializer& ObjectInitializer);
	//交互接口实现
	virtual void BeginOverlapTargetObject() override;
	virtual void EndOverlapTargetObject() override;
};
