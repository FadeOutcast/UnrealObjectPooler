// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolingSubsystem.h"

void UObjectPoolingSubsystem::InitializePool(TSubclassOf<AActor> ActorClass, int PoolSize)
{
	UWorld* World = GetWorld();

	if (!IsValid(GetWorld())) {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to start object pool when world isn't available"));
		return;
	}

	TArray<UPooledComponent*> NewPooledComponents;

	for (int i = 0; i < PoolSize; i++) {
		AActor* SpawnedActor= World->SpawnActor(ActorClass);
		UPooledComponent* PooledComponent = SpawnedActor->FindComponentByClass<UPooledComponent>();
		
		if (!PooledComponent) {
			UE_LOG(LogTemp, Warning, TEXT("Actor you are trying to spawn does not have the pooledcomponent"));
			return;
		}

		NewPooledComponents.Add(PooledComponent);

	}
	//Appends object pool if it already exists
	if (ObjectPools.Contains(ActorClass)) {
		ObjectPools[ActorClass].PooledComponents.Append(NewPooledComponents);
	}
	else {
		ObjectPools[ActorClass].PooledComponents = NewPooledComponents;
	}

}

UPooledComponent* UObjectPoolingSubsystem::GetFirstAvailablePooledComponent(const TSubclassOf<AActor>& ActorClass)
{
	if (!ObjectPools.Contains(ActorClass)) return nullptr;
	TArray<UPooledComponent*> PooledComponents = ObjectPools[ActorClass].PooledComponents;

	
	for(UPooledComponent* PooledComponent: PooledComponents)
	{
		if (!PooledComponent->IsActorActive()) return PooledComponent;

	}

	return nullptr;
}

TArray<UPooledComponent*> UObjectPoolingSubsystem::GetMultipleAvailablePooledComponents(const TSubclassOf<AActor>& ActorClass, const int Quantity)
{
	TArray<UPooledComponent*> Result;
	if (!ObjectPools.Contains(ActorClass) || Quantity <= 0) return Result;

	TArray<UPooledComponent*> PooledComponents = ObjectPools[ActorClass].PooledComponents;

	int counter = 0;


	for (UPooledComponent* PooledComponent : PooledComponents)
	{
		if (!PooledComponent->IsActorActive()) {
			Result.Add(PooledComponent);
			counter++;
			if (counter >= Quantity) return Result;
		}

	}

	// Returns all the available actors, even if couldn't reach the quantity requested
	return Result;
	
}


AActor* UObjectPoolingSubsystem::SpawnFromPool(const TSubclassOf<AActor>& ActorClass)
{
	UPooledComponent* PooledComponent = GetFirstAvailablePooledComponent(ActorClass);
	PooledComponent->SetActorActive();
	return PooledComponent->GetOwner();
}

TArray<AActor*> UObjectPoolingSubsystem::SpawnMultipleFromPool(const TSubclassOf<AActor>& ActorClass, const int Quantity)
{
	TArray<AActor*> SpawnedActors;
	if (Quantity <= 0) SpawnedActors;
	
	TArray<UPooledComponent*> PooledComponents = GetMultipleAvailablePooledComponents(ActorClass, Quantity);
	
	//Spawn Actors and add to both the pool and the array PooledComponents if the ones available in the pool were not enough
	if (PooledComponents.Num() < Quantity) {

		//Add logic to spawn actors
	}

	for (UPooledComponent* PooledComponent : PooledComponents) {
		PooledComponent->SetActorActive();
		SpawnedActors.Add(PooledComponent->GetOwner());
	}

	return SpawnedActors;
}
