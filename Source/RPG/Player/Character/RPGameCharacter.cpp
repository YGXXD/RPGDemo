// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGameCharacter.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "Paper2D/Classes/PaperSprite.h"
#include "RPG/AbilitySystem/AbilityUMG/SkillBar.h"
#include "RPG/AbilitySystem/Actor/Dash.h"
#include "RPG/AbilitySystem/Actor/DashInAir.h"
#include "RPG/AbilitySystem/Actor/MeleeA.h"
#include "RPG/AbilitySystem/Actor/MeleeB.h"
#include "RPG/AbilitySystem/Actor/MeleeC.h"
#include "RPG/AbilitySystem/Actor/MeleeD.h"
#include "RPG/AbilitySystem/Actor/MeleeE.h"
#include "RPG/AbilitySystem/Actor/MeleeInAirA.h"
#include "RPG/AbilitySystem/Actor/MeleeInAirB.h"
#include "RPG/AbilitySystem/Actor/MeleeInAirC.h"
#include "RPG/AbilitySystem/Actor/MeleeInAirD.h"
#include "RPG/AbilitySystem/Component/AbilityComponent.h"
#include "RPG/GameSystem/LoadManager.h"
#include "RPG/GameSystem/RPGSystemLibrary.h"
#include "RPG/NeutralObject/InteractionUMG/SelectTalkBox.h"
#include "RPG/Player/Actor/MiniMapCapture.h"
#include "RPG/Player/Animation/RPGameCharacterAnim.h"
#include "RPG/Player/Component/NumberWidgetComponent.h"
#include "RPG/Player/Controller/RPGamePlayerController.h"
#include "RPG/Player/ControllerUMG/CharacterBorder.h"
#include "RPG/Player/ControllerUMG/GlassBar.h"
#include "RPG/Player/ControllerUMG/LevelBar.h"
#include "RPG/Player/ControllerUMG/MiniMap.h"
#include "RPG/Player/ControllerUMG/PlayerControllerMainUI.h"
#include "RPG/QuestSystem/Component/QuestComponent.h"
#include "RPG/LoadSystem/SaveData/RPGSaveGame.h"


// Sets default values
ARPGameCharacter::ARPGameCharacter(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;
	JumpState = EJumpType::JumpFirst;
	
	//初始化胶囊提
	GetCapsuleComponent()->SetCapsuleHalfHeight(88);
	GetCapsuleComponent()->SetCapsuleRadius(38);

	//初始化Mesh
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetRelativeLocation(FVector(0,0,-90));
	GetMesh()->SetRelativeRotation(FRotator(0,-90,0));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GrimMesh(TEXT("/Game/RPG_Game/Character/SwordMan/Animation/Skeleton/Mesh_SwordMan"));
	if(GrimMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(GrimMesh.Object);
	}
	static ConstructorHelpers::FClassFinder<URPGameCharacterAnim> ABP_Grim(TEXT("/Game/RPG_Game/Character/SwordMan/Animation/ABP_SwordMan"));
	if(ABP_Grim.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ABP_Grim.Class);
	}

	SwordComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordComp"));
	SwordComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);;
	SwordComp->SetupAttachment(GetMesh());
	SwordComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordComp->SetOnlyOwnerSee(true);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SwordMesh(TEXT("/Game/RPG_Game/Character/SwordMan/mesh/Sword"));
	if(SwordMesh.Succeeded())
	{
		SwordComp->SetStaticMesh(SwordMesh.Object);
	}
	
	//初始化移动组件
	PlayerMovementComp = GetCharacterMovement();
	PlayerMovementComp->bOrientRotationToMovement = true;
	PlayerMovementComp->JumpZVelocity = 620;
	PlayerMovementComp->AirControl = 0.5;
	PlayerMovementComp->RotationRate.Yaw = 1440;
	PlayerMovementComp->FallingLateralFriction = 4;
	PlayerMovementComp->GroundFriction = 4;

	//初始化主相机遥感组件
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	SpringArmComp->TargetArmLength = 650;
	SpringArmComp->SetRelativeRotation(FRotator(-45,0,0));
	SpringArmComp->bInheritPitch = false;
	SpringArmComp->bInheritRoll = false;
	SpringArmComp->bInheritYaw = false;

	//初始化BoxComponent
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(GetCapsuleComponent());
	BoxComp->SetRelativeLocation(FVector(150,0,0));
	BoxComp->SetBoxExtent(FVector(180,40,40));
	BoxComp->CanCharacterStepUpOn = ECB_No;
	BoxComp->SetCollisionObjectType(ECC_WorldStatic);
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ARPGameCharacter::InteractBeginOverlapByBox);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ARPGameCharacter::InteractEndOverlapByBox);

	//初始化主相机
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	//初始化地图相机摇杆组件
	MiniMapSpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("MiniMapSpringArmComp"));
	MiniMapSpringArmComp->SetupAttachment(GetCapsuleComponent());
	MiniMapSpringArmComp->SetRelativeRotation(FRotator(-90,0,0));
	MiniMapSpringArmComp->TargetArmLength = 900;
	MiniMapSpringArmComp->bInheritPitch = false;
	MiniMapSpringArmComp->bInheritYaw = false;
	MiniMapSpringArmComp->bInheritRoll = false;
	MiniMapSpringArmComp->bDoCollisionTest = false;

	//初始化地图相机组件
	MiniMapCaptureComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("MiniMapCaptureComp"));
	MiniMapCaptureComp->SetupAttachment(MiniMapSpringArmComp);
	static ConstructorHelpers::FClassFinder<AMiniMapCapture> BP_MiniMapCapture(TEXT("/Game/RPG_Game/Blueprints/Player/BP_MiniMapCapture"));
	if(BP_MiniMapCapture.Succeeded())
	{
		MiniMapCaptureComp->SetChildActorClass(BP_MiniMapCapture.Class);
	}

	//初始化PaperSpriteCharacter组件
	PaperSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PaperSpriteComp"));
	PaperSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PaperSpriteComp->SetupAttachment(GetCapsuleComponent());
	static ConstructorHelpers::FObjectFinder<UPaperSprite> MiniMapCharacterSprite(TEXT("PaperSprite'/Game/RPG_Game/Textures/MiniMapCharacter_Sprite.MiniMapCharacter_Sprite'"));
	if(MiniMapCharacterSprite.Succeeded())
	{
		PaperSpriteComp->SetSprite(MiniMapCharacterSprite.Object);
	}
	PaperSpriteComp->SetSpriteColor(FLinearColor(1,1,0));
	PaperSpriteComp->SetRelativeLocation(FVector(0,0,140));
	PaperSpriteComp->SetRelativeRotation(FRotator(0,90,-90));
	PaperSpriteComp->SetRelativeScale3D(FVector(0.5,0.5,0.5));
	PaperSpriteComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PaperSpriteComp->SetOwnerNoSee(true);

	//初始化任务组件
	QuestComp = CreateDefaultSubobject<UQuestComponent>(TEXT("QuestComp"));

	//初始化技能组件
	AbilityComp = CreateDefaultSubobject<UAbilityComponent>(TEXT("AbilityComp"));
	
	//初始化默认类
	static ConstructorHelpers::FClassFinder<UUserWidget> WBP_PCMainUI(TEXT("/Game/RPG_Game/UI/WBP_PlayerControllerMain"));
	if(WBP_PCMainUI.Succeeded())
	{
		PCMainUIClass = WBP_PCMainUI.Class; 
	}

	static ConstructorHelpers::FClassFinder<UActorComponent> WC_NumberWidgetComponent(TEXT("/Game/RPG_Game/UI/WC_NumberWidgetComponent"));
	if(WC_NumberWidgetComponent.Succeeded())
	{
		NumberWidgetComponentClass = WC_NumberWidgetComponent.Class;
	}
	
	//构造函数内初始化角色血量，魔法等属性
	ConstructInitializeCharacter();
}

void ARPGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitializeCharacter();
	//添加插槽这里在编辑器启动时会报错
	SwordComp->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,EAttachmentRule::KeepRelative,true),
		TEXT("hand_rSocket"));
	PLAY_MONTAGE(GetMesh(),TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_SwordMan_Equip.AM_SwordMan_Equip'"));
}

void ARPGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

#pragma region 获取组件或属性

EJumpType& ARPGameCharacter::GetJumpState()
{
	return JumpState;
}

USpringArmComponent* ARPGameCharacter::GetSpringArmComponent() const
{
	return SpringArmComp;
}

UCharacterMovementComponent* ARPGameCharacter::GetPlayerMovementComponent() const
{
	return PlayerMovementComp;
}

UPlayerControllerMainUI* ARPGameCharacter::GetMainUI() const
{
	return PCMainUI;
}

UQuestComponent* ARPGameCharacter::GetQuestComponent() const
{
	return QuestComp;
}

UAbilityComponent* ARPGameCharacter::GetAbilityComponent() const
{
	return AbilityComp;
}

float ARPGameCharacter::GetCurrentHealth() const
{
	if(AttributeMap.Contains(EAttributeType::Health))
		return AttributeMap[EAttributeType::Health].CurrentValue;
	return 0;
}

float ARPGameCharacter::GetCurrentMana() const
{
	if(AttributeMap.Contains(EAttributeType::Mana))
		return AttributeMap[EAttributeType::Mana].CurrentValue;
	return 0;
}

float ARPGameCharacter::GetCurrentStamina() const
{
	if(AttributeMap.Contains(EAttributeType::Stamina))
		return AttributeMap[EAttributeType::Stamina].CurrentValue;
	return 0;
}

float ARPGameCharacter::GetCurrentLevel() const
{
	if(AttributeMap.Contains(EAttributeType::Level))
		return AttributeMap[EAttributeType::Level].CurrentValue;
	return 0;
}

#pragma endregion

#pragma region 移动组件启用和关闭

void ARPGameCharacter::EnableControllerInput()
{
	ARPGamePlayerController* PlayerController = Cast<ARPGamePlayerController>(GetController());
	if(PlayerController)
	{
		PlayerController->RegisterInput();
	}
}

void ARPGameCharacter::DisableControllerInput()
{
	ARPGamePlayerController* PlayerController = Cast<ARPGamePlayerController>(GetController());
	if(PlayerController)
	{
		PlayerController->UnRegisterInput();
	}
}

void ARPGameCharacter::DelayEnableControllerInput(float DelayTime)
{
	const FLatentActionInfo DelayInfo(1,2,TEXT("EnableControllerInput"),this);
	UKismetSystemLibrary::Delay(this, DelayTime, DelayInfo);
}

#pragma endregion

#pragma region 初始化角色属性函数

void ARPGameCharacter::ConstructInitializeCharacter()
{
	AddAttribute(EAttributeType::Health,FAttribute(300,0,300,1,120));
	AddAttribute(EAttributeType::Mana,FAttribute(400,0,400,1,90));
	AddAttribute(EAttributeType::Stamina,FAttribute(100,0,100,0.1,8));
	AddAttribute(EAttributeType::Experience,FAttribute(0,0,100));
	AddAttribute(EAttributeType::Level,FAttribute(1,1,99));
	AddAttribute(EAttributeType::Money,FAttribute(20,0,99999));
}

void ARPGameCharacter::InitializeCharacter()
{
	//初始化角色UI绑定属性，和属性回复
	CreatePlayerControllerMainUI();
	if(PCMainUI)
	{
		CharacterBorder = PCMainUI->GetCharacterBorder();
		AMiniMapCapture* MiniMapCapture = Cast<AMiniMapCapture>(MiniMapCaptureComp->GetChildActor());
		USelectTalkBox* SelectTalkBox = PCMainUI->GetSelectTalkBox();
		UTalkBoard* TalkBoard = PCMainUI->GetTalkBoard();
		if(QuestComp)
		{
			QuestComp->SetPlayerAndMainUI(this,PCMainUI);
		}
		if(AbilityComp)
		{
			AbilityComp->SetPlayer(this);
			AbilityComp->SetPlayerSkillBar(PCMainUI->GetSkillBar());
		}
		if(MiniMapCapture)
		{
			PCMainUI->GetMiniMap()->SetMiniMapCapture(MiniMapCapture);
		}
		if(SelectTalkBox && TalkBoard)
		{
			SelectTalkBox->SetTalkBoard(TalkBoard);
			SelectTalkBox->SetPlayer(this);
		}
		InitializeAbility();
		
		//有UI才能设定
		SetUpAttributeBar(EAttributeType::Health, PCMainUI->GetHealthBar());
		SetUpAttributeBar(EAttributeType::Mana, PCMainUI->GetManaBar());
		SetUpAttributeBar(EAttributeType::Stamina,nullptr);
		SetUpAttributeBar(EAttributeType::Experience, PCMainUI->GetLevelBar());
		SetUpAttributeBar(EAttributeType::Level, PCMainUI->GetLevelBar());
		SetUpAttributeBar(EAttributeType::Money,nullptr);
		SetAutoRegenAttribute(EAttributeType::Health);
		SetAutoRegenAttribute(EAttributeType::Mana);
		SetAutoRegenAttribute(EAttributeType::Stamina);
	}
	LevelUpUpdateQuest();
}

void ARPGameCharacter::CreatePlayerControllerMainUI()
{
	if(!PCMainUI)
	{
		PCMainUI = Cast<UPlayerControllerMainUI>(CreateWidget<UUserWidget>(GetWorld(),PCMainUIClass));
		PCMainUI->AddToViewport(0);
	}
}

void ARPGameCharacter::AddAttribute(EAttributeType Key, const FAttribute& Value)
{
	if(!AttributeMap.Contains(Key))
	{
		AttributeMap.Add(Key,Value);
	}
}

void ARPGameCharacter::SetUpAttributeBar(EAttributeType Key,class UUserWidget* AttributeBar)
{
	if(AttributeMap.Contains(Key))
	{
		AttributeMap[Key].AttributeWidget = AttributeBar;
		UpdateAttribute(Key);
	}
}

#pragma endregion

#pragma region 更新角色属性

void ARPGameCharacter::UpdateAttribute(EAttributeType Key)
{
	if(AttributeMap.Contains(Key))
	{
		switch (Key)
		{
		case EAttributeType::Health:
			UpdateHealth();
			break;
			
		case EAttributeType::Mana:
			UpdateMana();
			break;
			
		case EAttributeType::Stamina:
			UpdateStamina();
			break;
			
		case EAttributeType::Experience:
			UpdateExperience();
			break;

		case EAttributeType::Level:
			UpdateLevel();
			break;

		case EAttributeType::Money:
			UpdateMoney();
			break;
			
		default:
			break;
		}
	}
}

void ARPGameCharacter::UpdateHealth()
{
	UGlassBar* HealthBar = Cast<UGlassBar>(AttributeMap[EAttributeType::Health].AttributeWidget);
	if(HealthBar)
	{
		const FAttribute Attribute = AttributeMap[EAttributeType::Health];
		HealthBar->UpdateGlassBarValue(Attribute.CurrentValue,Attribute.MaxValue);
		HealthBar->UpdateGlassBarColor(Attribute.CurrentValue,Attribute.MaxValue,FLinearColor(1,0,0));
	}
	if(CharacterBorder)
	{
		CharacterBorder->UpdateCharacterBorder(EAttributeType::Health,AttributeMap[EAttributeType::Health].CurrentValue,AttributeMap[EAttributeType::Health].MaxValue);
	}
}

void ARPGameCharacter::UpdateMana()
{
	UGlassBar* ManaBar = Cast<UGlassBar>(AttributeMap[EAttributeType::Mana].AttributeWidget);
	if(ManaBar)
	{
		const FAttribute Attribute = AttributeMap[EAttributeType::Mana];
		ManaBar->UpdateGlassBarValue(Attribute.CurrentValue,Attribute.MaxValue);
	}
	if(CharacterBorder)
	{
		CharacterBorder->UpdateCharacterBorder(EAttributeType::Mana,AttributeMap[EAttributeType::Mana].CurrentValue,AttributeMap[EAttributeType::Mana].MaxValue);
	}
}

void ARPGameCharacter::UpdateStamina()
{
	if(CharacterBorder)
	{
		CharacterBorder->UpdateCharacterBorder(EAttributeType::Stamina,AttributeMap[EAttributeType::Stamina].CurrentValue,AttributeMap[EAttributeType::Stamina].MaxValue);
	}
}

void ARPGameCharacter::UpdateExperience()
{
	ULevelBar* LevelBar = Cast<ULevelBar>(AttributeMap[EAttributeType::Experience].AttributeWidget);
	if(LevelBar)
	{
		//显示经验条
		LevelBar->XpBarToView();
		
		//lambda
		auto XpUpdateLambda = [this,LevelBar]()
		{
			const FAttribute Experience = AttributeMap[EAttributeType::Experience];
			float XpPercent = LevelBar->GetXpPercent();
			XpPercent = FMath::Clamp<float>(XpPercent + 0.005,0,Experience.CurrentValue/Experience.MaxValue);
			LevelBar->SetXpPercent(XpPercent);

			//判断是否升级了
			if(XpPercent >= 1)
			{
				LevelBar->SetXpPercent(0);
				LevelUp();
			}
		
			if(XpPercent == Experience.CurrentValue/Experience.MaxValue)
			{
				//延时10秒隐藏经验条
				LevelBar->DelayXpBarToHide(10);
				LevelBar->PauseUpdateTimer();
			}
		};
		const FTimerDelegate XpUpdateDelegate = FTimerDelegate::CreateLambda(XpUpdateLambda);
		LevelBar->SetUpdateTimer(XpUpdateDelegate,0.01,true);
	}
}

void ARPGameCharacter::UpdateLevel()
{
	ULevelBar* LevelBar = Cast<ULevelBar>(AttributeMap[EAttributeType::Experience].AttributeWidget);
	if(LevelBar)
	{
		LevelBar->UpdateLevelValue(AttributeMap[EAttributeType::Level].CurrentValue);
	}
	if(CharacterBorder)
	{
		CharacterBorder->UpdateCharacterBorder(EAttributeType::Level,AttributeMap[EAttributeType::Level].CurrentValue,AttributeMap[EAttributeType::Level].MaxValue);
	}
}

void ARPGameCharacter::UpdateMoney()
{
	if(CharacterBorder)
	{
		CharacterBorder->UpdateCharacterBorder(EAttributeType::Money,AttributeMap[EAttributeType::Money].CurrentValue,AttributeMap[EAttributeType::Money].MaxValue);
	}
}

void ARPGameCharacter::ModifyAttribute(EAttributeType Key, float ModifyValue, bool bIsSpawnNumText)
{
	if(AttributeMap.Contains(Key))
	{
		const FAttribute Attribute = AttributeMap[Key];
		switch (Key)
		{
		case EAttributeType::Health:
			ModifyValue = FMath::Clamp<float>(ModifyValue,-Attribute.CurrentValue,Attribute.MaxValue - Attribute.CurrentValue);
			AttributeMap[Key].CurrentValue += ModifyValue;
			RegenTimerHandelActivate(Key);
			break;
			
		case EAttributeType::Mana:
			ModifyValue = FMath::Clamp<float>(ModifyValue,-Attribute.CurrentValue,Attribute.MaxValue - Attribute.CurrentValue);
			AttributeMap[Key].CurrentValue += ModifyValue;
			RegenTimerHandelActivate(Key);
			break;

		case EAttributeType::Stamina:
			ModifyValue = FMath::Clamp<float>(ModifyValue,-Attribute.CurrentValue,Attribute.MaxValue - Attribute.CurrentValue);
			AttributeMap[Key].CurrentValue += ModifyValue;
			RegenTimerHandelActivate(Key);
			break;
			
		case EAttributeType::Experience:
			if(ModifyValue <= 0)
			{
				return;
			}
			AttributeMap[Key].CurrentValue += ModifyValue;
			break;
			
		case EAttributeType::Level:
			if(ModifyValue <= 0)
			{
				break;
			}
			AttributeMap[Key].CurrentValue =
				FMath::Clamp<float>(Attribute.CurrentValue + ModifyValue, Attribute.MinValue, Attribute.MaxValue);
			break;

		case EAttributeType::Money:
			if(Attribute.CurrentValue + ModifyValue >= 0)
			{
				AttributeMap[Key].CurrentValue =
				FMath::Clamp<float>(Attribute.CurrentValue + ModifyValue, Attribute.MinValue, Attribute.MaxValue);
			}
		default:
			break;
		}
		if(bIsSpawnNumText)
		{
			SpawnModifyNumUI(Key,ModifyValue);
		}
		UpdateAttribute(Key);
	}
}

void ARPGameCharacter::SpawnModifyNumUI(EAttributeType Key, float ModifyValue)
{
	UActorComponent* Component = AddComponentByClass(NumberWidgetComponentClass,true,GetActorTransform(),false);
	UNumberWidgetComponent* NumberWidgetComponent = Cast<UNumberWidgetComponent>(Component);
	if(NumberWidgetComponent)
	{
		NumberWidgetComponent->AddNumberTextToView(Key, ModifyValue);
	}
}

#pragma endregion

#pragma region 升级逻辑实现

void ARPGameCharacter::LevelUp()
{
	LevelUpExperience(1.5);
	LevelUpHealth(100,100);
	LevelUpMana(70,70);
	LevelUpStamina(2,2);
	LevelUpLevel(1);
	
	DisableControllerInput();
	const float DelayTime = PLAY_MONTAGE(GetMesh(),
		TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_SwordMan_LevelUp.AM_SwordMan_LevelUp'"));
	DelayEnableControllerInput(DelayTime);
}

void ARPGameCharacter::LevelUpHealth(float CurrentValue, float MaxValue)
{
	if(AttributeMap.Contains(EAttributeType::Health))
	{
		AttributeMap[EAttributeType::Health].MaxValue += MaxValue;
		AttributeMap[EAttributeType::Health].CurrentValue =
			FMath::Clamp<float>(AttributeMap[EAttributeType::Health].CurrentValue + CurrentValue,
								AttributeMap[EAttributeType::Health].MinValue,
								AttributeMap[EAttributeType::Health].MaxValue);
		UpdateAttribute(EAttributeType::Health);
	}
}

void ARPGameCharacter::LevelUpMana(float CurrentValue, float MaxValue)
{
	if(AttributeMap.Contains(EAttributeType::Mana))
	{
		AttributeMap[EAttributeType::Mana].MaxValue += MaxValue;
		AttributeMap[EAttributeType::Mana].CurrentValue =
			FMath::Clamp<float>(AttributeMap[EAttributeType::Mana].CurrentValue + CurrentValue,
			                    AttributeMap[EAttributeType::Mana].MinValue,
			                    AttributeMap[EAttributeType::Mana].MaxValue);
		UpdateAttribute(EAttributeType::Mana);
	}
}

void ARPGameCharacter::LevelUpStamina(float CurrentValue, float MaxValue)
{
	if(AttributeMap.Contains(EAttributeType::Stamina))
	{
		AttributeMap[EAttributeType::Stamina].MaxValue += MaxValue;
		AttributeMap[EAttributeType::Stamina].CurrentValue =
			FMath::Clamp<float>(AttributeMap[EAttributeType::Stamina].CurrentValue + CurrentValue,
								AttributeMap[EAttributeType::Stamina].MinValue,
								AttributeMap[EAttributeType::Stamina].MaxValue);
		UpdateAttribute(EAttributeType::Stamina);
	}
}

void ARPGameCharacter::LevelUpExperience(float MaxValueIncreaseRate)
{
	if(AttributeMap.Contains(EAttributeType::Experience))
	{
		AttributeMap[EAttributeType::Experience].CurrentValue -= AttributeMap[EAttributeType::Experience].MaxValue;
		AttributeMap[EAttributeType::Experience].MaxValue *= MaxValueIncreaseRate;
	}
}

void ARPGameCharacter::LevelUpLevel(float CurrentValue, float MaxValue)
{
	if(AttributeMap.Contains(EAttributeType::Level))
	{
		AttributeMap[EAttributeType::Level].MaxValue += MaxValue;
		AttributeMap[EAttributeType::Level].CurrentValue =
			FMath::Clamp<float>(AttributeMap[EAttributeType::Level].CurrentValue + CurrentValue,
								AttributeMap[EAttributeType::Level].MinValue,
								AttributeMap[EAttributeType::Level].MaxValue);
		SpawnModifyNumUI(EAttributeType::Level);
		LevelUpUpdateQuest();
		UpdateAttribute(EAttributeType::Level);
	}
}

#pragma endregion

#pragma region 回复系统

void ARPGameCharacter::SetAutoRegenAttribute(EAttributeType Key)
{
	if(AttributeMap.Contains(Key))
	{
		float RegenValue =
			AttributeMap[Key].RegenRate * AttributeMap[Key].MaxValue * AttributeMap[Key].RegenInterval / AttributeMap[Key].RegenMaxTime;
	
		auto RegenLambda = [this, RegenValue, Key]()
		{
			ModifyAttribute(Key, RegenValue);
		};
		const FTimerDelegate RegenDelegate = FTimerDelegate::CreateLambda(RegenLambda);
		GetWorldTimerManager().SetTimer(AttributeMap[Key].RegenHandle, RegenDelegate, AttributeMap[Key].RegenInterval, true);
		RegenTimerHandelActivate(Key);
	}
}

void ARPGameCharacter::RegenTimerHandelActivate(EAttributeType Key)
{
	if (AttributeMap[Key].CurrentValue == AttributeMap[Key].MaxValue &&
			!GetWorldTimerManager().IsTimerPaused(AttributeMap[Key].RegenHandle))
	{
		GetWorldTimerManager().PauseTimer(AttributeMap[Key].RegenHandle);
	}
	else if(AttributeMap[Key].CurrentValue != AttributeMap[Key].MaxValue &&
		GetWorldTimerManager().IsTimerPaused(AttributeMap[Key].RegenHandle))
	{
		GetWorldTimerManager().UnPauseTimer(AttributeMap[Key].RegenHandle);
	}
}

#pragma endregion 

#pragma region 跳跃

void ARPGameCharacter::CharacterJump()
{
	switch (JumpState)
	{
	case EJumpType::JumpFirst:
		FirstJump();
		break;

	case EJumpType::JumpDouble:
		SecondJump();
		break;
		
	default:
		break;
	}
}

void ARPGameCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	JumpState = EJumpType::JumpFirst;
}

void ARPGameCharacter::FirstJump()
{
	Jump();
	//PLAY_AUDIO(this,GetWorld(),GetActorLocation(),
		//TEXT("SoundCue'/Game/RPG_Game/Character/Shinbi/ShinbiAudio/Shinbi_Effort_Jump.Shinbi_Effort_Jump'"));
	JumpState = EJumpType::JumpDouble;
}

void ARPGameCharacter::SecondJump()
{
	LaunchCharacter(FVector(0,0,600),false,true);
	//PLAY_AUDIO(this,GetWorld(),GetActorLocation(),
		//TEXT("SoundCue'/Game/RPG_Game/Character/Shinbi/ShinbiAudio/Shinbi_Effort_Jump.Shinbi_Effort_Jump'"));
	JumpState = EJumpType::JumpNone;
}

#pragma endregion

#pragma region 多播委托

void ARPGameCharacter::Interact() const
{
	OnInteract.Broadcast();
}

void ARPGameCharacter::LevelUpUpdateQuest() const
{
	if(AttributeMap.Contains(EAttributeType::Level))
	{
		OnLevelUpUpdateQuest.Broadcast(AttributeMap[EAttributeType::Level].CurrentValue);
	}
}

#pragma endregion

#pragma region Box检测物体是否可以交互

void ARPGameCharacter::InteractBeginOverlapByBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(OtherActor);
	if (InteractionInterface)
	{
		InteractionInterface->BeginOverlapTargetObject();
	}
}

void ARPGameCharacter::InteractEndOverlapByBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IInteractionInterface* InteractionInterface = Cast<IInteractionInterface>(OtherActor);
	if (InteractionInterface)
	{
		InteractionInterface->EndOverlapTargetObject();
	}
}

#pragma endregion

#pragma region 角色任务接口

void ARPGameCharacter::AcceptQuest(TSubclassOf<AMasterQuest> QuestClass)
{
	if(QuestComp)
	{
		QuestComp->AddQuest(QuestClass);
	}
}

void ARPGameCharacter::LearnAbility(TSubclassOf<AAbilityBase> AbilityClass)
{
	if(AbilityComp)
	{
		AbilityComp->AddAbility(AbilityClass);
	}
}

void ARPGameCharacter::SwitchMainMissionBorder()
{
	if(PCMainUI)
	{
		PCMainUI->SwitchMainMissionBorder();
	}
}

#pragma endregion

#pragma region 角色技能接口

void ARPGameCharacter::InitializeAbility() const
{
	if(AbilityComp)
	{
		AbilityComp->AddAbility(AMeleeA::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeB::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeC::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeD::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeE::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeInAirA::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeInAirB::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeInAirC::StaticClass(),true);
		AbilityComp->AddAbility(AMeleeInAirD::StaticClass(),true);
		AbilityComp->AddAbility(ADash::StaticClass(),true);
		AbilityComp->AddAbility(ADashInAir::StaticClass(),true);
	}
}

void ARPGameCharacter::ReleaseAbility(FString AbilityName) const
{
	if(AbilityComp)
	{
		AbilityComp->TryActivateAbility(AbilityName);
	}
}

void ARPGameCharacter::MeleeMBR() const
{
	if(PlayerMovementComp->IsFalling())
	{
		ReleaseAbility("MeleeInAirA");
	}
	else
	{
		ReleaseAbility("MeleeA");
	}
}

void ARPGameCharacter::MeleeMBL() const
{
	if(PlayerMovementComp->IsFalling())
	{
		ReleaseAbility("MeleeInAirD");
	}
	else
	{
		ReleaseAbility("MeleeD");
	}
}

void ARPGameCharacter::Dash() const
{
	if(PlayerMovementComp->IsFalling())
	{
		ReleaseAbility("DashInAir");
	}
	else
	{
		ReleaseAbility("Dash");
	}
}

void ARPGameCharacter::ReleaseSKill(int SkillImageListID)
{
	if(PCMainUI)
	{
		USkillBar* SkillBar = PCMainUI->GetSkillBar();
		if(SkillBar)
		{
			const FString SkillName = SkillBar->GetSkillNameInImageList(SkillImageListID);
			if(SkillName == "")
			{
				UE_LOG(LogTemp,Warning,TEXT("没有该技能"));
				return;
			}
			ReleaseAbility(SkillName);
		}
	}
}

void ARPGameCharacter::SetPlayerLookAtView()
{
	FMinimalViewInfo CameraView;
	CameraComp->GetCameraView(GetWorld()->GetDeltaSeconds(),CameraView);
	FRotator MeleeRotation = CameraView.Rotation;
	MeleeRotation.Roll = GetActorRotation().Roll;
	MeleeRotation.Pitch = GetActorRotation().Pitch;
	SetActorRotation(MeleeRotation);
}

#pragma endregion

#pragma region 角色受伤接口

void ARPGameCharacter::TakeDamageForCharacter(float Value)
{
	if(AttributeMap[EAttributeType::Health].CurrentValue <= AttributeMap[EAttributeType::Health].MinValue)
	{
		return;
	}
	TakeDamageWithEffect();
	ModifyAttribute(EAttributeType::Health,-Value,true);
	if(AttributeMap[EAttributeType::Health].CurrentValue <= AttributeMap[EAttributeType::Health].MinValue)
	{
		CharacterToDie();
	}
}

void ARPGameCharacter::TakeDamageWithEffect()
{
	PLAY_MONTAGE(GetMesh(),TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_SwordMan_Hit.AM_SwordMan_Hit'"));
}

void ARPGameCharacter::CharacterToDie()
{
	UE_LOG(LogTemp,Warning,TEXT("DIE"));
	for (const auto& AttributeElem : AttributeMap)
	{
		if(!GetWorldTimerManager().IsTimerPaused(AttributeElem.Value.RegenHandle))
		{
			GetWorldTimerManager().PauseTimer(AttributeElem.Value.RegenHandle);
		}
	}
	DisableControllerInput();
	PlayerMovementComp->AddImpulse(-GetActorForwardVector()*500);
	PLAY_MONTAGE(GetMesh(),TEXT("AnimMontage'/Game/RPG_Game/Character/SwordMan/Animation/Anim/AM_SwordMan_Dead.AM_SwordMan_Dead'"));
	if(ARPGamePlayerController* PC = Cast<ARPGamePlayerController>(GetController()))
	{
		PC->SetShowMouseCursor(true);
	}
	if(PCMainUI)
	{
		PCMainUI->GameOver();
	}
}

#pragma endregion

#pragma region 保存角色功能

void ARPGameCharacter::SwitchGameMenu()
{
	if(PCMainUI)
	{
		PCMainUI->SwitchGameMenu();
	}
}

void ARPGameCharacter::SaveInfo()
{
	SAVE_DATA->SavePlayerAttribute(AttributeMap);
	SAVE_DATA->SavePlayerTransform(GetActorTransform());
	if(AbilityComp)
	{
		AbilityComp->SaveAbility();
	}
	if(QuestComp)
	{
		QuestComp->SaveQuest();
	}
}

void ARPGameCharacter::LoadInfo(int Index)
{
	SAVE_DATA->LoadPlayerAttribute(AttributeMap,Index);
	for (const auto& Elem : AttributeMap)
	{
		UpdateAttribute(Elem.Key);
	}
	RegenTimerHandelActivate(EAttributeType::Health);
	RegenTimerHandelActivate(EAttributeType::Mana);
	RegenTimerHandelActivate(EAttributeType::Stamina);
	SetActorTransform(SAVE_DATA->GetPlayerTransform(Index));
	if(AbilityComp)
	{
		AbilityComp->LoadAbility(Index);
	}
	if(QuestComp)
	{
		QuestComp->LoadQuest(Index);
	}
}

#pragma endregion