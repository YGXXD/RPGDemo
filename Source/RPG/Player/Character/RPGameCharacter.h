// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RPG/Player/Interface/AbilityAttackInterface.h"
#include "RPG/Player/Interface/SaveLoadInterface.h"
#include "RPGameCharacter.generated.h"


UENUM()
enum class EJumpType : uint8
{
	JumpNone, //不能跳跃
	JumpFirst, //可以跳跃第一段
	JumpDouble //可以二段跳
};

UENUM()
enum class EAttributeType : uint8
{
	Health,
	Mana,
	Stamina,
	Experience,
	Level,
	Money
};

USTRUCT()
struct FAttribute
{
	GENERATED_USTRUCT_BODY()

	FAttribute():
		CurrentValue(0),
		MinValue(0),
		MaxValue(0),
		RegenInterval(0),
		RegenMaxTime(0),
		RegenRate(0),
		AttributeWidget(nullptr)
	{
	}

	FAttribute(float CurrentValue, float MinValue, float MaxValue):
		CurrentValue(CurrentValue),
		MinValue(MinValue),
		MaxValue(MaxValue),
		RegenInterval(0),
		RegenMaxTime(0),
		RegenRate(0),
		AttributeWidget(nullptr)
	{
	}

	FAttribute(float CurrentValue, float MinValue, float MaxValue, float RegenInterval, float RegenMaxTime, float RegenRate = 1):
		CurrentValue(CurrentValue),
		MinValue(MinValue),
		MaxValue(MaxValue),
		RegenInterval(RegenInterval),
		RegenMaxTime(RegenMaxTime),
		RegenRate(RegenRate),
		AttributeWidget(nullptr)
	{
	}
	
	float CurrentValue;
	float MinValue;
	float MaxValue;
	
	//自动回复属性
	float RegenInterval;
	float RegenMaxTime;
	float RegenRate;
	FTimerHandle RegenHandle;
	
	UPROPERTY()
	class UUserWidget* AttributeWidget;
};

//角色与场景NPC等交互的多播委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestMapUpdateDelegate, int, Level);

UCLASS()
class RPG_API ARPGameCharacter : public ACharacter, public IAbilityAttackInterface, public ISaveLoadInterface
{
	GENERATED_BODY()
	
public:
	ARPGameCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	EJumpType JumpState;//提供给动画调用
	
	UPROPERTY()
	class UCharacterMovementComponent* PlayerMovementComp;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* SwordComp;

	//用于检测是否碰到任务的目标物
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	//MINI地图控制相机的摇杆臂
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* MiniMapSpringArmComp;

	UPROPERTY(VisibleAnywhere)
	class UChildActorComponent* MiniMapCaptureComp;

	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* PaperSpriteComp;

	//自定义的任务组件
	UPROPERTY()
	class UQuestComponent* QuestComp;

	//自定义的技能组件
	UPROPERTY()
	class UAbilityComponent* AbilityComp;
	
	UPROPERTY()
	TMap<EAttributeType,FAttribute> AttributeMap;

	UPROPERTY()
	TSubclassOf<class UUserWidget> PCMainUIClass;

	UPROPERTY()
	TSubclassOf<class UActorComponent> NumberWidgetComponentClass;

	UPROPERTY()
	class UPlayerControllerMainUI* PCMainUI;

	UPROPERTY()
	class UCharacterBorder* CharacterBorder;
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
private:
	//初始化角色属性
	void ConstructInitializeCharacter();//构造函数初始化角色属性
	void InitializeCharacter();//开始时初始化角色UI和属性功能
	void CreatePlayerControllerMainUI();
	void AddAttribute(EAttributeType Key,const FAttribute& Value);
	void SetUpAttributeBar(EAttributeType Key,class UUserWidget* AttributeBar);
	//属性更新的主要逻辑
	void UpdateAttribute(EAttributeType Key);
	void UpdateHealth();
	void UpdateMana();
	void UpdateStamina();
	void UpdateExperience();
	void UpdateLevel();
	void UpdateMoney();
	void SpawnModifyNumUI(EAttributeType Key, float ModifyValue = 0);
	//升级
	void LevelUp();
	void LevelUpHealth(float CurrentValue, float MaxValue);//升级血量改变
	void LevelUpMana(float CurrentValue, float MaxValue);//升级魔法改变
	void LevelUpStamina(float CurrentValue, float MaxValue);//升级耐力改变
	void LevelUpExperience(float MaxValueIncreaseRate);//升级经验值改变
	void LevelUpLevel(float CurrentValue, float MaxValue = 0);//升级等级改变
	//自动回复系统
	void SetAutoRegenAttribute(EAttributeType Key);
	void RegenTimerHandelActivate(EAttributeType Key);
	//延迟激活输入
	void DelayEnableControllerInput(float DelayTime);
	//跳跃以及二段跳逻辑，跳跃逻辑CharacterJump()在public中
	virtual void Landed(const FHitResult& Hit) override;//重写父类Character，着陆事件函数
	void FirstJump();
	void SecondJump();
	//用于Box组件检测物体是否可以交互
	UFUNCTION()
	void InteractBeginOverlapByBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
								const FHitResult& SweepResult);
	UFUNCTION()
	void InteractEndOverlapByBox(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//技能
	void InitializeAbility() const;//初始化角色技能
	void ReleaseAbility(FString AbilityName) const;
	//死亡接口
	virtual void CharacterToDie() override;
	
public:
	//角色委托
	FOnInteractDelegate OnInteract;//交互多播委托
	FOnQuestMapUpdateDelegate OnLevelUpUpdateQuest;//角色升级时更新地图上NPC接任务的信息委托,更新任务
	

	USpringArmComponent* GetSpringArmComponent() const;
	UCharacterMovementComponent* GetPlayerMovementComponent() const;
	UPlayerControllerMainUI* GetMainUI() const;//获取主UI
	EJumpType& GetJumpState();

	//激活和禁用输入
	UFUNCTION()
	void DisableControllerInput();
	UFUNCTION()
	void EnableControllerInput();
	
	//改变角色属性，如血量，魔法值，经验。。。。。改变值为属性当前值CurrentValue
	void ModifyAttribute(EAttributeType Key, float ModifyValue, bool bIsSpawnNumText = false);
	//获取角色属性,血量，魔法值，体力的当前值
	float GetCurrentHealth() const;
	float GetCurrentMana() const;
	float GetCurrentStamina() const;
	float GetCurrentLevel() const;
	//角色跳跃逻辑，绑定控制器输入
	void CharacterJump();
	//交互,多播交互委托,绑定控制器输入
	void Interact() const;
	//多播地图上NPC接任务信息委托和自身等级信息
	void LevelUpUpdateQuest() const;
	//任务
	UQuestComponent* GetQuestComponent() const;
	void AcceptQuest(TSubclassOf<class AMasterQuest> QuestClass);
	void SwitchMainMissionBorder();//开关任务面板
	//技能
	UAbilityComponent* GetAbilityComponent() const;
	void LearnAbility(TSubclassOf<class AAbilityBase> AbilityClass);
	void MeleeMBR() const;
	void MeleeMBL() const;
	void SetPlayerLookAtView();
	void Dash() const;
	void ReleaseSKill(int SkillImageListID);
	//伤害接口
	virtual void TakeDamageForCharacter(float Value) override;
	virtual void TakeDamageWithEffect() override;
	//保存角色
	void SwitchGameMenu();//开关任务面板
	virtual void SaveInfo() override;
	virtual void LoadInfo(int Index) override;
};
