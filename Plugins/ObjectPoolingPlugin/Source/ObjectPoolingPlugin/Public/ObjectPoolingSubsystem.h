// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PooledComponent.h"
#include "ObjectPoolingSubsystem.generated.h"

/**
 * 
 */
USTRUCT()
struct FObjectPoolData {

	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	TArray<UPooledComponent*> PooledComponents;
};

UCLASS()
class OBJECTPOOLINGPLUGIN_API UObjectPoolingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void InitializePool(TSubclassOf<AActor> ActorClass, int PoolSize);

	//UFUNCTION(BlueprintCallable)
	//AActor* GetFirstAvailableActor(TSubclassOf<AActor> ActorClass);

	UFUNCTION(BlueprintCallable)
	UPooledComponent* GetFirstAvailablePooledComponent(const TSubclassOf<AActor>& ActorClass);

	UFUNCTION(BlueprintCallable)
	TArray<UPooledComponent*> GetMultipleAvailablePooledComponents(const TSubclassOf<AActor>& ActorClass, const int Quantity);

	UFUNCTION(BlueprintCallable)
	AActor* SpawnFromPool(const TSubclassOf<AActor>& ActorClass);

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> SpawnMultipleFromPool(const TSubclassOf<AActor>& ActorClass, const int Quantity);

protected:
	//UPROPERTY()
	//TArray<FObjectPoolData> ObjectPools;

	UPROPERTY()
	TMap<TSubclassOf<AActor>, FObjectPoolData> ObjectPools;
};
