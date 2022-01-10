// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGSystemLibrary.h"

#include "ActorPool.h"
#include "WidgetPool.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

void URPGSystemLibrary::PlayAudio(const UObject* WorldContextObject, FVector Location, const TCHAR* Path)
{
	USoundCue* SoundCue = LoadObject<USoundCue>(nullptr,Path);
	if(SoundCue)
	{
		UGameplayStatics::PlaySoundAtLocation(WorldContextObject,SoundCue,Location);
	}
}

float URPGSystemLibrary::PlayAnimMontage(const USkeletalMeshComponent* SkeletalMeshComponent, const TCHAR* Path)
{
	UAnimMontage* AnimMontage = LoadObject<UAnimMontage>(nullptr,Path);
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	float MontageLength = 0;
	if(AnimMontage && AnimInstance)
	{
		MontageLength = AnimInstance->Montage_Play(AnimMontage);
	}
	return MontageLength;
}

float URPGSystemLibrary::PlayAnimMontage(UAnimInstance* AnimInstance, UAnimMontage* AnimMontage)
{
	float MontageLength = 0;
	if(AnimMontage && AnimInstance)
	{
		MontageLength = AnimInstance->Montage_Play(AnimMontage);
	}
	return MontageLength;
}

bool URPGSystemLibrary::IsAnimMontageStop(const USkeletalMeshComponent* SkeletalMeshComponent, const TCHAR* Path)
{
	const UAnimMontage* AnimMontage = LoadObject<UAnimMontage>(nullptr,Path);
	UAnimInstance* AnimInstance = SkeletalMeshComponent->GetAnimInstance();
	if(AnimInstance && AnimMontage)
	{
		return AnimInstance->Montage_GetIsStopped(AnimMontage);
	}
	return false;
}

bool URPGSystemLibrary::IsAnimMontageStop(UAnimInstance* AnimInstance, const UAnimMontage* AnimMontage)
{
	if(AnimInstance && AnimMontage)
	{
		return AnimInstance->Montage_GetIsStopped(AnimMontage);
	}
	return false;
}

void URPGSystemLibrary::StopAllMontage(UAnimInstance* AnimInstance, float BlendOut)
{
	if(AnimInstance)
	{
		AnimInstance->StopAllMontages(BlendOut);
	}
}

void URPGSystemLibrary::OpenLevel(const UWorld* World, FName LevelName)
{
	EMPTY_WIDGET_POOL;
	EMPTY_ACTOR_POOL;
	UGameplayStatics::OpenLevel(World,LevelName);
}
