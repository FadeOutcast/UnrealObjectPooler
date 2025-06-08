// Fill out your copyright notice in the Description page of Project Settings.


#include "PooledComponent.h"

// Sets default values for this component's properties
UPooledComponent::UPooledComponent() : bIsActive(false)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}



// Called when the game starts
void UPooledComponent::BeginPlay()
{
	Super::BeginPlay();

	SetActorInactive();

	UE_LOG(LogTemp, Display, TEXT("STARTED"));

	// ...
	
}


// Never called as tick is disabled
void UPooledComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

bool UPooledComponent::IsActorActive()
{
	return bIsActive;
}


void UPooledComponent::SetActorActive_Implementation()
{
	//UE_LOG(LogTemp, Display, TEXT("ACTIVATED"));
	bIsActive = true;
	GetOwner()->SetActorHiddenInGame(false);
	GetOwner()->SetActorEnableCollision(true);
	// Only enable tick if it was enabled by default
	bool bActorTick = GetOwner()->PrimaryActorTick.bStartWithTickEnabled;
	GetOwner()->SetActorTickEnabled(bActorTick);
}

void UPooledComponent::SetActorInactive_Implementation()
{
	//UE_LOG(LogTemp, Display, TEXT("DEACTIVATED"));
	bIsActive = false;
	GetOwner()->SetActorHiddenInGame(true);
	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorTickEnabled(false);
}