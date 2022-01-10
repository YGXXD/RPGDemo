// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetPool.h"

#include "Blueprint/UserWidget.h"
#include "Components/PanelWidget.h"

UWidgetPool* UWidgetPool::WidgetPoolInstance = nullptr;

UWidgetPool::UWidgetPool(const FObjectInitializer& ObjectInitializer) : UObject(ObjectInitializer)
{
	
}

UWidgetPool* UWidgetPool::GetInstance()
{
	if(!WidgetPoolInstance)
	{
		WidgetPoolInstance = NewObject<UWidgetPool>();
		WidgetPoolInstance->AddToRoot();
	}
	return WidgetPoolInstance;
}

void UWidgetPool::DestroyWidgetPool()
{
	if(WidgetPoolInstance)
	{
		WidgetPoolInstance->RemoveFromRoot();
		WidgetPoolInstance = nullptr;
	}
}

void UWidgetPool::AddWidgetToPoolMap(TSubclassOf<UUserWidget> WidgetClass)
{
	if(WidgetClass && !WidgetPoolMap.Contains(WidgetClass))
	{
		const FWidgetPoolItem WidgetPool;
		WidgetPoolMap.Add(WidgetClass,WidgetPool);
	}
}

void UWidgetPool::RemoveWidgetFromPoolMap(TSubclassOf<UUserWidget> WidgetClass)
{
	if(WidgetClass && WidgetPoolMap.Contains(WidgetClass))
	{
		WidgetPoolMap.Remove(WidgetClass);
	}
}

void UWidgetPool::EmptyWidgetPool()
{
	WidgetPoolMap.Empty();
}

template <typename WidgetT>
WidgetT* UWidgetPool::GetUserWidgetFromUMGPool(TSubclassOf<UUserWidget> WidgetClass,UWorld* World)
{
	WidgetT* Widget = nullptr;
	if(WidgetPoolMap.Contains(WidgetClass))
	{
		if (WidgetPoolMap[WidgetClass].Pool.Num())
		{
			UE_LOG(LogTemp,Warning,TEXT("1"))
			Widget = Cast<WidgetT>(WidgetPoolMap[WidgetClass].Pool[0]);
			WidgetPoolMap[WidgetClass].Pool.RemoveAt(0);
		}
		else
		{
			Widget = CreateWidget<WidgetT>(World, WidgetClass);
		}
	}

	return Widget;
}

template <typename WidgetT>
void UWidgetPool::RemoveWidgetAllChildren(TSubclassOf<UUserWidget> WidgetClass,UPanelWidget* ParentWidget)
{
	if(WidgetPoolMap.Contains(WidgetClass))
	{
		const TArray<UWidget*> WidgetList = ParentWidget->GetAllChildren();
		for (int i = 0; i < WidgetList.Num(); i++)
		{
			if(Cast<WidgetT>(WidgetList[i]))
			{
				WidgetPoolMap[WidgetClass].Pool.Add(Cast<WidgetT>(WidgetList[i]));
			}
		}
		ParentWidget->ClearChildren();
	}
}

template <typename WidgetT>
void UWidgetPool::EmptyWidgetList(TSubclassOf<UUserWidget> WidgetClass,TArray<WidgetT*>& WidgetArray)
{
	if(WidgetPoolMap.Contains(WidgetClass))
	{
		for (int i = 0; i < WidgetArray.Num(); i++)
		{
			WidgetPoolMap[WidgetClass].Pool.Add(WidgetArray[i]);
		}
		WidgetArray.Empty();
	}
}
