// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGamePlayerController.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "RPG/AbilitySystem/Actor/GorgeousSlash.h"
#include "RPG/AbilitySystem/Actor/HealthRegen.h"
#include "RPG/AbilitySystem/Actor/WildStabbing.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/Player/Actor/MoveTarget.h"
#include "RPG/Player/Character/RPGameCharacter.h"
#include "RPG/QuestSystem/Component/QuestComponent.h"

ARPGamePlayerController::ARPGamePlayerController(const FObjectInitializer& ObjectInitializer)
{
	bShowMouseCursor = false;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	bIsMouseMove = false;
	
	static ConstructorHelpers::FClassFinder<AMoveTarget> BP_TargetDecal(TEXT("/Game/RPG_Game/Blueprints/Player/BP_MoveTargetActor"));
	if(BP_TargetDecal.Succeeded())
	{
		TargetDecalClass = BP_TargetDecal.Class;
	}
}

void ARPGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = Cast<ARPGameCharacter>(GetPawn());
	if(PlayerPawn)
	{
		PlayerSpringArm = PlayerPawn->GetSpringArmComponent();
	}
	CreateMoveTargetActor();
	RegisterInput();
}

void ARPGamePlayerController::CreateMoveTargetActor()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	MoveTargetDecal = GetWorld()->SpawnActor<AMoveTarget>(TargetDecalClass,SpawnParams);
	if(MoveTargetDecal)
	{
		//将MoveTarget激活属性设置为false
		MoveTargetDecal->SetActorActive(false);
	}
}

void ARPGamePlayerController::RegisterInput()
{
	InputComponent->BindAction("ShowMouse",IE_Pressed,this,&ARPGamePlayerController::ShowMouseCursor);
	InputComponent->BindAction("MoveToLocation",IE_Pressed,this,&ARPGamePlayerController::MouseMove);
	InputComponent->BindAction("Jump",IE_Pressed,this,&ARPGamePlayerController::CharacterJump);
	InputComponent->BindAction("Interact",IE_Pressed,this,&ARPGamePlayerController::Interact);
	InputComponent->BindAction("MeleeMBR",IE_Pressed,this,&ARPGamePlayerController::MeleeMBR);
	InputComponent->BindAction("MeleeMBL",IE_Pressed,this,&ARPGamePlayerController::MeleeMBL);
	InputComponent->BindAction("Dash",IE_Pressed,this,&ARPGamePlayerController::Dash);
	InputComponent->BindAction("SkillOne",IE_Pressed,this,&ARPGamePlayerController::ReleaseSkillOne);
	InputComponent->BindAction("SkillTwo",IE_Pressed,this,&ARPGamePlayerController::ReleaseSkillTwo);
	InputComponent->BindAction("SkillThree",IE_Pressed,this,&ARPGamePlayerController::ReleaseSkillThree);
	InputComponent->BindAction("SkillFour",IE_Pressed,this,&ARPGamePlayerController::ReleaseSkillFour);
	InputComponent->BindAction("SkillFive",IE_Pressed,this,&ARPGamePlayerController::ReleaseSkillFive);
	InputComponent->BindAction("SwitchMainMission",IE_Pressed,this,&ARPGamePlayerController::SwitchMainMissionBorder);
	InputComponent->BindAction("SwitchGameMenu",IE_Pressed,this,&ARPGamePlayerController::SwitchGameMenu);
	
	InputComponent->BindAxis("LookUp",this,&ARPGamePlayerController::LookUp);
	InputComponent->BindAxis("Turn",this,&ARPGamePlayerController::Turn);
	InputComponent->BindAxis("MoveForward",this,&ARPGamePlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight",this,&ARPGamePlayerController::MoveRight);
	InputComponent->BindAxis("ViewSizeUp",this,&ARPGamePlayerController::ViewSizeUp);

	////////////
	InputComponent->BindAction("ClickTest",IE_Pressed,this,&ARPGamePlayerController::GetXp);
	InputComponent->BindAction("Save",IE_Pressed,this,&ARPGamePlayerController::Save);
}

void ARPGamePlayerController::UnRegisterInput()
{
	InputComponent->ClearActionBindings();

	for(int i = 0;i < InputComponent->AxisBindings.Num();i++)
	{
		InputComponent->AxisBindings[i].AxisDelegate.Unbind();
	}
}

void ARPGamePlayerController::ShowMouseCursor()
{
	SetShowMouseCursor(!bShowMouseCursor);
}

void ARPGamePlayerController::MouseMove()
{
	if(bIsMouseMove)
	{
		CancelMouseMove();
	}
	bIsMouseMove = true;
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility,true,HitResult);
	//激活MoveTargetActor，设置他的旋转和坐标
	MoveTargetDecal->SetActorLocation(HitResult.Location);
	MoveTargetDecal->SetActorRotation(FRotator::ZeroRotator);
	MoveTargetDecal->SetActorActive(true);
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(this,HitResult.Location);
}

void ARPGamePlayerController::CancelMouseMove()
{
	if (MoveTargetDecal && PlayerPawn)
	{
		PlayerPawn->GetPlayerMovementComponent()->StopMovementImmediately();
		//GetWorld()->DestroyActor(MoveTargetDecal);
		//如果取消MouseMove，将目的地标记的MoveTarget激活属性设置false
		MoveTargetDecal->SetActorActive(false);
		bIsMouseMove = false;
	}
}

void ARPGamePlayerController::MoveForward(float Value)
{
	if(!Value)
	{
		return;;
	}
	if(bIsMouseMove)
	{
		CancelMouseMove();
	}
	if(PlayerPawn && PlayerSpringArm)
	{
		const FRotator PlayerSpringArmRotation = PlayerSpringArm->GetRelativeRotation();
		const FRotator PlayerRotation(0,PlayerSpringArmRotation.Yaw,0);
		const FVector Direction = UKismetMathLibrary::GetForwardVector(PlayerRotation);
		PlayerPawn->AddMovementInput(Direction,Value);
		if(PlayerPawn->GetQuestComponent())
		{
			PlayerPawn->GetQuestComponent()->UpdateTargetOnMove();
		}
	}
}

void ARPGamePlayerController::MoveRight(float Value)
{
	if(!Value)
	{
		return;;
	}
	if(bIsMouseMove)
	{
		CancelMouseMove();
	}
	if(PlayerPawn && PlayerSpringArm)
	{
		const FRotator PlayerSpringArmRotation = PlayerSpringArm->GetRelativeRotation();
		const FRotator PlayerRotation(0,PlayerSpringArmRotation.Yaw + 90,0);
		const FVector Direction = UKismetMathLibrary::GetForwardVector(PlayerRotation);
		PlayerPawn->AddMovementInput(Direction,Value);
		if(PlayerPawn->GetQuestComponent())
		{
			PlayerPawn->GetQuestComponent()->UpdateTargetOnMove();
		}
	}
}

void ARPGamePlayerController::LookUp(float Value)
{
	if(!Value)
	{
		return;;
	}
	if(PlayerSpringArm)
	{
		FRotator RelativeRotation = PlayerSpringArm->GetRelativeRotation();
		RelativeRotation.Pitch += Value * 1.5;
		RelativeRotation.Pitch = FMath::Clamp<float>(RelativeRotation.Pitch,-45,0);
		PlayerSpringArm->SetRelativeRotation(RelativeRotation);
	}
}

void ARPGamePlayerController::Turn(float Value)
{
	if(!Value)
	{
		return;;
	}
	if(PlayerSpringArm)
	{
		FRotator RelativeRotation = PlayerSpringArm->GetRelativeRotation();
		RelativeRotation.Yaw += Value * 2;
		PlayerSpringArm->SetRelativeRotation(RelativeRotation);
	}
}

void ARPGamePlayerController::ViewSizeUp(float Value)
{
	if(PlayerSpringArm)
	{
		PlayerSpringArm->TargetArmLength = FMath::Clamp<float>(PlayerSpringArm->TargetArmLength += Value * 15,150,1500);
	}
}

void ARPGamePlayerController::CharacterJump()
{
	if(PlayerPawn)
	{
		PlayerPawn->CharacterJump();
	}
}

void ARPGamePlayerController::Interact()
{
	if(PlayerPawn)
	{
		PlayerPawn->Interact();
	}
}

void ARPGamePlayerController::MeleeMBR()
{
	if(PlayerPawn)
	{
		PlayerPawn->MeleeMBR();
	}
}

void ARPGamePlayerController::MeleeMBL()
{
	if(PlayerPawn)
	{
		PlayerPawn->MeleeMBL();
	}
}

void ARPGamePlayerController::Dash()
{
	if(PlayerPawn)
	{
		PlayerPawn->Dash();
	}
}

void ARPGamePlayerController::ReleaseSkillOne()
{
	if(PlayerPawn)
	{
		PlayerPawn->ReleaseSKill(0);
	}
}

void ARPGamePlayerController::ReleaseSkillTwo()
{
	if(PlayerPawn)
	{
		PlayerPawn->ReleaseSKill(1);
	}
}

void ARPGamePlayerController::ReleaseSkillThree()
{
	if(PlayerPawn)
	{
		PlayerPawn->ReleaseSKill(2);
	}
}

void ARPGamePlayerController::ReleaseSkillFour()
{
	if(PlayerPawn)
	{
		PlayerPawn->ReleaseSKill(3);
	}
}

void ARPGamePlayerController::ReleaseSkillFive()
{
	if(PlayerPawn)
	{
		PlayerPawn->ReleaseSKill(4);
	}
}

void ARPGamePlayerController::SwitchMainMissionBorder()
{
	if(PlayerPawn)
	{
		PlayerPawn->SwitchMainMissionBorder();
	}
}

void ARPGamePlayerController::SwitchGameMenu()
{
	if(PlayerPawn)
	{
		PlayerPawn->SwitchGameMenu();
	}
}


////////////////////////
void ARPGamePlayerController::GetXp()
{
	if(PlayerPawn)
	{   
		//PlayerPawn->TakeDamageForCharacter(50);
		// PlayerPawn->ModifyAttribute(EAttributeType::Mana,-30);
		// PlayerPawn->ModifyAttribute(EAttributeType::Stamina,-30);
		// PlayerPawn->ModifyAttribute(EAttributeType::Experience,50);
		// PlayerPawn->ModifyAttribute(EAttributeType::Money,100);
		// PlayerPawn->GetQuestComponent()->CompleteSubGoal(ID);
		// ID++;
		PlayerPawn->ModifyAttribute(EAttributeType::Experience,100);
		PlayerPawn->LearnAbility(AWildStabbing::StaticClass());
		PlayerPawn->LearnAbility(AGorgeousSlash::StaticClass());
		PlayerPawn->LearnAbility(AHealthRegen::StaticClass());
	}
}

void ARPGamePlayerController::Save()
{
	SAVE_MANAGER->SaveGame(GetWorld());
}

