// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RPGamePlayerController.generated.h"

class AMoveTarget;
UCLASS()
class RPG_API ARPGamePlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	bool bIsMouseMove;
	
	UPROPERTY()
	TSubclassOf<class AMoveTarget> TargetDecalClass; 

	UPROPERTY()
	class ARPGameCharacter* PlayerPawn;

	UPROPERTY()
	class USpringArmComponent* PlayerSpringArm;

	UPROPERTY()
	class AMoveTarget* MoveTargetDecal;

	ARPGamePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	
	//创建一个MoveTargetActor，用来标记目的地
	void CreateMoveTargetActor();
	//输入绑定的函数
	void ShowMouseCursor();
	void MouseMove();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void ViewSizeUp(float Value);
	void CharacterJump();
	void Interact();
	void MeleeMBR();
	void MeleeMBL();
	void Dash();
	void ReleaseSkillOne();
	void ReleaseSkillTwo();
	void ReleaseSkillThree();
	void ReleaseSkillFour();
	void ReleaseSkillFive();
	void SwitchMainMissionBorder();
	void SwitchGameMenu();
	
	//////////////////////////////
	void GetXp();
	void Save();
	int ID = 0;
public:
	//绑定输入和解绑输入
	void RegisterInput();
	void UnRegisterInput();
	//取消鼠标点击的移动指令
	void CancelMouseMove();
	
};

