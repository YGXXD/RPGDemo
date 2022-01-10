// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WidgetPool.generated.h"

#define WIDGET_POOL UWidgetPool::GetInstance()
#define CREATE_WIDGET_POOL UWidgetPool::GetInstance()->AddWidgetToPoolMap
#define DELETE_WIDGET_POOL UWidgetPool::GetInstance()->RemoveWidgetFromPoolMap
#define EMPTY_WIDGET_POOL UWidgetPool::GetInstance()->EmptyWidgetPool()
#define DESTROY_WIDGET_POOL UWidgetPool::DestroyWidgetPool()
/**
 * 
 */

USTRUCT()
struct FWidgetPoolItem
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<class UUserWidget*> Pool;
};

UCLASS()
class RPG_API UWidgetPool : public UObject
{
	GENERATED_BODY()
private:
	static class UWidgetPool* WidgetPoolInstance;
	
	UPROPERTY()//防止被GC
	TMap<TSubclassOf<class UUserWidget>,FWidgetPoolItem> WidgetPoolMap;
	
	UWidgetPool(const FObjectInitializer& ObjectInitializer);
	
public:
	static UWidgetPool* GetInstance();
	static void DestroyWidgetPool();

	void AddWidgetToPoolMap(TSubclassOf<class UUserWidget> WidgetClass);
	void RemoveWidgetFromPoolMap(TSubclassOf<class UUserWidget> WidgetClass);
	void EmptyWidgetPool();
	
	template <typename WidgetT = UUserWidget>
	WidgetT* GetUserWidgetFromUMGPool(TSubclassOf<class UUserWidget> WidgetClass,class UWorld* World);

	template <typename WidgetT = UUserWidget>
	void RemoveWidgetAllChildren(TSubclassOf<class UUserWidget> WidgetClass,class UPanelWidget* ParentWidget);

	template <typename WidgetT = UUserWidget>
	void EmptyWidgetList(TSubclassOf<class UUserWidget> WidgetClass,TArray<WidgetT*>& WidgetArray);
};
