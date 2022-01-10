// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class AEnemyBase* AnimOwner;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_EnableMelee(class UAnimNotify* EnableMeleeNotify);

protected:
	UPROPERTY(BlueprintReadOnly)
	float Speed;

public:
	UEnemyAnim();
};
