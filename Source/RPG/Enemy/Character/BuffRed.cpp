// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffRed.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Enemy/Animation/EnemyAnim.h"

ABuffRed::ABuffRed(const FObjectInitializer& ObjectInitializer) : AEnemyBase(ObjectInitializer)
{
	ExpValue = 60;
	
	GetCapsuleComponent()->SetCapsuleRadius(70);
	GetCapsuleComponent()->SetCapsuleHalfHeight(110);

	EnemyMovementComp->MaxWalkSpeed = 350;
	
	EnemyMesh->SetRelativeLocation(FVector(0,0,-110));
	EnemyMesh->SetRelativeRotation(FRotator(0,-90,0));

	StateWidgetComp->SetRelativeLocation(FVector(0,0,120));
	
	static ConstructorHelpers::FClassFinder<UEnemyAnim> ABP_BuffRed(TEXT("/Game/RPG_Game/Character/Buff_Red/Animation/ABP_BuffRedAnim"));
	if(ABP_BuffRed.Succeeded())
	{
		EnemyMesh->SetAnimInstanceClass(ABP_BuffRed.Class);
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BuffRedMesh(TEXT("SkeletalMesh'/Game/RPG_Game/Character/Buff_Red/Meshes/Buff_Red.Buff_Red'"));
	if(BuffRedMesh.Succeeded())
	{
		EnemyMesh->SetSkeletalMesh(BuffRedMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Red/Animation/AM_BuffRed_Melee.AM_BuffRed_Melee'"));
	if(MeleeAnim.Succeeded())
	{
		MeleeAttackMontage = MeleeAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpawnAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Red/Animation/AM_BuffRed_Spawn.AM_BuffRed_Spawn'"));
	if(SpawnAnim.Succeeded())
	{
		SpawnMontage = SpawnAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Red/Animation/AM_BuffRed_Hit.AM_BuffRed_Hit'"));
	if(HitAnim.Succeeded())
	{
		HitMontage = HitAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Red/Animation/AM_BuffRed_Death.AM_BuffRed_Death'"));
	if(DeathAnim.Succeeded())
	{
		DeathMontage = DeathAnim.Object;
	}
	
}

void ABuffRed::InitializeEnemy()
{
	Super::InitializeEnemy();
	ResetHealth(600);
}

void ABuffRed::CharacterToDie()
{
	Super::CharacterToDie();
}
