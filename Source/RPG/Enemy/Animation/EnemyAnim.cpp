// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "RPG/Enemy/Character/EnemyBase.h"
#include "RPG/Player/Character/RPGameCharacter.h"

UEnemyAnim::UEnemyAnim()
{
	Speed = 0;	
}

void UEnemyAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	APawn* Pawn = TryGetPawnOwner();
	if(Pawn)
	{
		AnimOwner = Cast<AEnemyBase>(Pawn);
	}
}

void UEnemyAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(AnimOwner)
	{
		Speed = UKismetMathLibrary::VSize(AnimOwner->GetVelocity());
	}
}

void UEnemyAnim::AnimNotify_EnableMelee(UAnimNotify* EnableMeleeNotify)
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
	UKismetSystemLibrary::BoxTraceMultiForObjects(GetWorld(), Pos, Pos, FVector(40,40,40), BoxTraceRotation,
										 ObjectTypes,false,IgnoreActorList, EDrawDebugTrace::ForDuration, HitResultList, true,
										FLinearColor::Black, FLinearColor::Yellow, 10);
	
	for (int i = 0; i < HitResultList.Num(); i++)
	{
		AActor* Actor = HitResultList[i].GetActor();
		if(ARPGameCharacter* Player = Cast<ARPGameCharacter>(Actor))
		{
			Player->TakeDamageForCharacter(35);
		}
	}
}
