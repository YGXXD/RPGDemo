// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffBlue.h"

#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "RPG/Enemy/Animation/EnemyAnim.h"

ABuffBlue::ABuffBlue(const FObjectInitializer& ObjectInitializer) : AEnemyBase(ObjectInitializer)
{
	ExpValue = 20;
	
	GetCapsuleComponent()->SetCapsuleRadius(25);
	GetCapsuleComponent()->SetCapsuleHalfHeight(25);
	
	EnemyMesh->SetRelativeLocation(FVector(0,0,-55));
	EnemyMesh->SetRelativeRotation(FRotator(0,-90,0));

	StateWidgetComp->SetDrawSize(FVector2D(100,10));
	StateWidgetComp->SetRelativeLocation(FVector(0,0,40));
	
	static ConstructorHelpers::FClassFinder<UEnemyAnim> ABP_BuffBlue(TEXT("/Game/RPG_Game/Character/Buff_Blue/Animation/ABP_BuffBlue"));
	if(ABP_BuffBlue.Succeeded())
	{
		EnemyMesh->SetAnimInstanceClass(ABP_BuffBlue.Class);
	}
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> BuffBlueMesh(TEXT("SkeletalMesh'/Game/RPG_Game/Character/Buff_Blue/Meshes/Buff_Blue.Buff_Blue'"));
	if(BuffBlueMesh.Succeeded())
	{
		EnemyMesh->SetSkeletalMesh(BuffBlueMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Blue/Animation/AM_BuffBlue_Melee.AM_BuffBlue_Melee'"));
	if(MeleeAnim.Succeeded())
	{
		MeleeAttackMontage = MeleeAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SpawnAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Blue/Animation/AM_BuffBlue_Spawn.AM_BuffBlue_Spawn'"));
	if(SpawnAnim.Succeeded())
	{
		SpawnMontage = SpawnAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HitAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Blue/Animation/AM_BuffBlue_Hit.AM_BuffBlue_Hit'"));
	if(HitAnim.Succeeded())
	{
		HitMontage = HitAnim.Object;
	}
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeathAnim(TEXT("AnimMontage'/Game/RPG_Game/Character/Buff_Blue/Animation/AM_BuffBlue_Death.AM_BuffBlue_Death'"));
	if(DeathAnim.Succeeded())
	{
		DeathMontage = DeathAnim.Object;
	}
	
}

void ABuffBlue::InitializeEnemy()
{
	Super::InitializeEnemy();
	EnemyMovementComp->SetMovementMode(MOVE_Flying);
}

void ABuffBlue::CharacterToDie()
{
	Super::CharacterToDie();
}
