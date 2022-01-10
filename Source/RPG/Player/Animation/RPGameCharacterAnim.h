// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RPGameCharacterAnim.generated.h"

/**
 * 
 */
UCLASS()
class RPG_API URPGameCharacterAnim : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	class ARPGameCharacter* AnimOwner;
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	
	UPROPERTY(BlueprintReadOnly)
	FVector Speed;

	UPROPERTY(BlueprintReadOnly)
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly)
	bool bIsJumpFirst;

	UPROPERTY(BlueprintReadOnly)
	bool bIsJumpSecond;
	
	UPROPERTY()
	class AAbilityBase* CurrentAbility;

	bool bAbilityNeedFullBody;
	
	UFUNCTION()
	void AnimNotify_EnableMelee(class UAnimNotify* EnableMeleeNotify);

	UFUNCTION()
	void AnimNotify_EnableMeleeInAir(class UAnimNotify* EnableMeleeInAirNotify);

	UFUNCTION()
	void AnimNotify_EnableMeleeBlowAway(class UAnimNotify* EnableMeleeBlowAwayNotify);

	UFUNCTION()
	void AnimNotify_EnableMeleeFallAway(class UAnimNotify* EnableMeleeFallAwayNotify);

	UFUNCTION()
	void AnimNotify_EnableMeleeHitAway(class UAnimNotify* EnableMeleeHitAwayNotify);

	TArray<class IAbilityAttackInterface*> BoxTraceMultiForAbilityInterface(FVector BoxSize);

public:
	URPGameCharacterAnim();
	void SetCurrentAbility(class AAbilityBase* const Ability);
};
