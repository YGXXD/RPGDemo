#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RPGSystemLibrary.generated.h"

#define PLAY_AUDIO URPGSystemLibrary::PlayAudio
#define PLAY_MONTAGE URPGSystemLibrary::PlayAnimMontage
#define OPEN_LEVEL URPGSystemLibrary::OpenLevel
/**
 * 
 */
UCLASS()
class RPG_API URPGSystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	//播放声音
	static void PlayAudio(const UObject* WorldContextObject, FVector Location, const TCHAR* Path);
	//播放动画蒙太奇
	static float PlayAnimMontage(const USkeletalMeshComponent* SkeletalMeshComponent, const TCHAR* Path);
	static float PlayAnimMontage(class UAnimInstance* AnimInstance, class UAnimMontage* AnimMontage);
	static bool IsAnimMontageStop(const USkeletalMeshComponent* SkeletalMeshComponent, const TCHAR* Path);
	static bool IsAnimMontageStop(class UAnimInstance* AnimInstance, const class UAnimMontage* AnimMontage);
	static void StopAllMontage(UAnimInstance* AnimInstance, float BlendOut);
	//打开地图
	static void OpenLevel(const UWorld* World, FName LevelName);
};