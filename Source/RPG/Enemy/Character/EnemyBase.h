// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPG/Player/Interface/AbilityAttackInterface.h"
#include "EnemyBase.generated.h"

USTRUCT()
struct FEnemyAttribute
{
	GENERATED_BODY()
	float CurrentValue;
	float MaxValue;
	float MinValue;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthUpdateDelegate, float, HeathPercent);
UCLASS()
class RPG_API AEnemyBase : public ACharacter, public IAbilityAttackInterface
{
	GENERATED_BODY()


protected:
	FEnemyAttribute Health;
	FTimerHandle DelayDeleteHandle;
	FTimerHandle HideStateHandle;

	float ExpValue;
	
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* StateWidgetComp;
	
	UPROPERTY()
	class APatrolPath* PatrolPath;

	UPROPERTY()
	class UCharacterMovementComponent* EnemyMovementComp;

	UPROPERTY()
	class USkeletalMeshComponent* EnemyMesh;

	UPROPERTY()
	class UAnimMontage* MeleeAttackMontage;

	UPROPERTY()
	class UAnimMontage* SpawnMontage;

	UPROPERTY()
	class UAnimMontage* HitMontage;

	UPROPERTY()
	class UAnimMontage* DeathMontage;
	
	virtual void BeginPlay() override;
	void ResetHealth(float CurrentValue, float MaxValue = 100, float MinValue = 0);
	void DelayDeleteEnemy(float DelayTime);
	
	virtual void CharacterToDie() override;

public:
	FOnHealthUpdateDelegate HealthUpdate;
	AEnemyBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	UCharacterMovementComponent* GetEnemyMovementComponent() const;
	APatrolPath* GetPatrolPath() const;
	UAnimMontage* GetMeleeAttackMontage() const;
	virtual void MeleeAttack();
	
	virtual void InitializeEnemy();
	//伤害接口
	virtual void TakeDamageForCharacter(float Value) override;
	virtual void TakeDamageWithEffect() override;
	

};
