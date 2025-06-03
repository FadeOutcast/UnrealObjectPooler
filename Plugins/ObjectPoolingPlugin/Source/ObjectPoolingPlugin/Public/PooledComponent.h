// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OBJECTPOOLINGPLUGIN_API UPooledComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPooledComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsActive;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetActorActive();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetActorInactive();

	UFUNCTION(BlueprintCallable)
	bool IsActorActive();
};
