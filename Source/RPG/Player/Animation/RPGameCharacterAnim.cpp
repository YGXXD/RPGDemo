// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameCharacterAnim.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/AbilitySystem/Actor/AbilityBase.h"
#include "RPG/Enemy/Character/EnemyBase.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/Player/Interface/AbilityAttackInterface.h"

URPGameCharacterAnim::URPGameCharacterAnim()
{
	bIsInAir = false;
	CurrentAbility = nullptr;
	bIsJumpFirst = false;
	bIsJumpSecond = false;
}

void URPGameCharacterAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	APawn* Pawn = TryGetPawnOwner();
	if(Pawn)
	{
		AnimOwner = Cast<ARPGameCharacter>(Pawn);
	}
}

void URPGameCharacterAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(AnimOwner)
	{
		Speed = UKismetMathLibrary::InverseTransformDirection(AnimOwner->GetMesh()->GetComponentTransform(),AnimOwner->GetVelocity());
		bIsInAir = AnimOwner->GetPlayerMovementComponent()->IsFalling();
		bIsJumpFirst = AnimOwner->GetJumpState() == EJumpType::JumpDouble;
		bIsJumpSecond = AnimOwner->GetJumpState() == EJumpType::JumpNone;
	}
}

void URPGameCharacterAnim::SetCurrentAbility(AAbilityBase* const Ability)
{
	CurrentAbility = Ability;
}

TArray<IAbilityAttackInterface*> URPGameCharacterAnim::BoxTraceMultiForAbilityInterface(FVector BoxSize)
{
	TArray<AActor*> IgnoreActorList;
	IgnoreActorList.Add(AnimOwner);
	const FVector OwnerLocation = AnimOwner->GetActorLocation();
	const FVector OwnerForwardVector = AnimOwner->GetActorForwardVector();
	const FVector Pos = OwnerLocation + OwnerForwardVector * 150;
	const FRotator BoxTraceRotation = AnimOwner->GetActorRotation();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectTypeQuery3);
	TArray<FHitResult> HitResultList;
	UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), Pos, Pos, BoxSize, BoxTraceRotation,
										 ObjectTypes,false,IgnoreActorList, EDrawDebugTrace::ForDuration, HitResultList, true,
										FLinearColor::Black, FLinearColor::Yellow, 10);

	TArray<IAbilityAttackInterface*> IAbilityAttackInterfaceList;
	for (int i = 0; i < HitResultList.Num(); i++)
	{
		AActor* Actor = HitResultList[i].GetActor();
		IAbilityAttackInterface* AttackDamageInterface = Cast<IAbilityAttackInterface>(Actor);
		if(AttackDamageInterface)
		{
			IAbilityAttackInterfaceList.Add(AttackDamageInterface);
		}
	}

	return IAbilityAttackInterfaceList;
}

void URPGameCharacterAnim::AnimNotify_EnableMelee(UAnimNotify* EnableMeleeNotify)
{
	PLAY_AUDIO(GetWorld(),AnimOwner->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordSwing_Cue.SwordSwing_Cue'"));
	TArray<IAbilityAttackInterface*> AbilityAttackInterfaceList =
		BoxTraceMultiForAbilityInterface(FVector(70,50,40));
	for (int i = 0; i < AbilityAttackInterfaceList.Num(); i++)
	{
		if (CurrentAbility)
		{
			CurrentAbility->AbilityVfxEffect(AbilityAttackInterfaceList[i]);
			if(const AEnemyBase* Enemy = Cast<AEnemyBase>(AbilityAttackInterfaceList[i]))
			{
				PLAY_AUDIO(GetWorld(),Enemy->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordHit_Cue.SwordHit_Cue'"));
				FVector HitDirection = Enemy->GetActorLocation() - AnimOwner->GetActorLocation();
				HitDirection.Z = 0;
				const FVector HitImpulse = HitDirection.GetSafeNormal() * 5000;
				Enemy->GetEnemyMovementComponent()->AddImpulse(HitImpulse);
			}
			//伤害会导致死亡，所以后执行
			CurrentAbility->AbilityDamageEffect(AbilityAttackInterfaceList[i]);
		}
	}
}

void URPGameCharacterAnim::AnimNotify_EnableMeleeInAir(UAnimNotify* EnableMeleeInAirNotify)
{
	PLAY_AUDIO(GetWorld(),AnimOwner->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordSwingAir_Cue_Cue.SwordSwingAir_Cue_Cue'"));
	TArray<IAbilityAttackInterface*> AbilityAttackInterfaceList =
		BoxTraceMultiForAbilityInterface(FVector(70,50,40));

	for (int i = 0; i < AbilityAttackInterfaceList.Num(); i++)
	{
		if (CurrentAbility)
		{
			CurrentAbility->AbilityVfxEffect(AbilityAttackInterfaceList[i]);
			if(AEnemyBase* Enemy = Cast<AEnemyBase>(AbilityAttackInterfaceList[i]))
			{
				PLAY_AUDIO(GetWorld(),Enemy->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordHit_Cue.SwordHit_Cue'"));
				Enemy->LaunchCharacter(FVector(0,0,300),false,true);
				FVector HitDirection = Enemy->GetActorLocation() - AnimOwner->GetActorLocation();
				HitDirection.Z = 0;
				const FVector HitImpulse = HitDirection.GetSafeNormal() * 5000;
				Enemy->GetEnemyMovementComponent()->AddImpulse(HitImpulse);
			}
			//伤害会导致死亡，所以后执行
			CurrentAbility->AbilityDamageEffect(AbilityAttackInterfaceList[i]);
		}
	}
}

void URPGameCharacterAnim::AnimNotify_EnableMeleeBlowAway(UAnimNotify* EnableMeleeBlowAwayNotify)
{
	PLAY_AUDIO(GetWorld(),AnimOwner->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordSwing_Cue.SwordSwing_Cue'"));

	TArray<IAbilityAttackInterface*> AbilityAttackInterfaceList =
		BoxTraceMultiForAbilityInterface(FVector(70,40,70));

	for (int i = 0; i < AbilityAttackInterfaceList.Num(); i++)
	{
		if (CurrentAbility)
		{
			CurrentAbility->AbilityVfxEffect(AbilityAttackInterfaceList[i]);
			if(AEnemyBase* Enemy = Cast<AEnemyBase>(AbilityAttackInterfaceList[i]))
			{
				PLAY_AUDIO(GetWorld(),Enemy->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordHit_Cue.SwordHit_Cue'"));
				Enemy->LaunchCharacter(FVector(0,0,700),false,true);
			}
			//伤害会导致死亡，所以后执行
			CurrentAbility->AbilityDamageEffect(AbilityAttackInterfaceList[i]);
		}
	}
}

void URPGameCharacterAnim::AnimNotify_EnableMeleeFallAway(UAnimNotify* EnableMeleeFallAwayNotify)
{
	PLAY_AUDIO(GetWorld(),AnimOwner->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordSwingAir_Cue_Cue.SwordSwingAir_Cue_Cue'"));

	TArray<IAbilityAttackInterface*> AbilityAttackInterfaceList =
		BoxTraceMultiForAbilityInterface(FVector(70,40,70));

	for (int i = 0; i < AbilityAttackInterfaceList.Num(); i++)
	{
		if (CurrentAbility)
		{
			CurrentAbility->AbilityVfxEffect(AbilityAttackInterfaceList[i]);
			if(AEnemyBase* Enemy = Cast<AEnemyBase>(AbilityAttackInterfaceList[i]))
			{
				PLAY_AUDIO(GetWorld(),Enemy->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordHit_Cue.SwordHit_Cue'"));
				Enemy->LaunchCharacter(FVector(0,0,-1000),false,true);
			}
			//伤害会导致死亡，所以后执行
			CurrentAbility->AbilityDamageEffect(AbilityAttackInterfaceList[i]);
		}
	}
}

void URPGameCharacterAnim::AnimNotify_EnableMeleeHitAway(UAnimNotify* EnableMeleeHitAwayNotify)
{
	PLAY_AUDIO(GetWorld(),AnimOwner->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordSwing_Cue.SwordSwing_Cue'"));

	TArray<IAbilityAttackInterface*> AbilityAttackInterfaceList =
		BoxTraceMultiForAbilityInterface(FVector(50,60,40));

	for (int i = 0; i < AbilityAttackInterfaceList.Num(); i++)
	{
		if (CurrentAbility)
		{
			CurrentAbility->AbilityVfxEffect(AbilityAttackInterfaceList[i]);
			if(AEnemyBase* Enemy = Cast<AEnemyBase>(AbilityAttackInterfaceList[i]))
			{
				PLAY_AUDIO(GetWorld(),Enemy->GetActorLocation(),TEXT("SoundCue'/Game/RPG_Game/Character/SwordMan/Sound/SwordHit_Cue.SwordHit_Cue'"));
				FVector HitDirection = Enemy->GetActorLocation() - AnimOwner->GetActorLocation();
				HitDirection.Z = 0;
				HitDirection = HitDirection.GetSafeNormal();
				HitDirection = HitDirection * 50000;
				HitDirection.Z = 100;
				Enemy->LaunchCharacter(HitDirection,true,true);
			}
			//伤害会导致死亡，所以后执行
			CurrentAbility->AbilityDamageEffect(AbilityAttackInterfaceList[i]);
		}
	}
}
