// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveTarget.generated.h"

UCLASS()
class RPG_API AMoveTarget : public AActor
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	UDecalComponent* DecalComp;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* SceneComp;

public:	
	AMoveTarget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	void SetActorActive(bool Active);

	UFUNCTION()
	void BoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
