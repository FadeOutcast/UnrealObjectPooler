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
		if (!IsValid(SpawnedActor)) {
			UE_LOG(LogTemp, Warning, TEXT("Failed to spawn actor from class %s"), *ActorClass->GetName());
			return;
		}
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
		ObjectPools.Add(ActorClass, FObjectPoolData { NewPooledComponents });
	}

}

void UObjectPoolingSubsystem::DeletePool(TSubclassOf<AActor> ActorClass)
{
	if (!ObjectPools.Contains(ActorClass)) return;

	TArray<UPooledComponent*>& PooledComponents = ObjectPools[ActorClass].PooledComponents;

	for (UPooledComponent* PooledComponent : PooledComponents)
	{
		if (!PooledComponent) continue;

		AActor* Owner = PooledComponent->GetOwner();
		if (IsValid(Owner))
		{
			Owner->Destroy();
		}
	}

	ObjectPools.Remove(ActorClass);

}

UPooledComponent* UObjectPoolingSubsystem::GetFirstAvailablePooledComponent(const TSubclassOf<AActor>& ActorClass)
{
	if (!ObjectPools.Contains(ActorClass)) return nullptr;
	TArray<UPooledComponent*> &PooledComponents = ObjectPools[ActorClass].PooledComponents;

	
	for(UPooledComponent* &PooledComponent: PooledComponents)
	{
		if (!PooledComponent) {
			UE_LOG(LogTemp, Warning, TEXT("PoolComponent is null"));
			continue;
		}

		if (!PooledComponent->IsActorActive()) return PooledComponent;

	}

	return nullptr;
}

//void UObjectPoolingSubsystem::GetMultipleAvailablePooledComponents(const TSubclassOf<AActor>& ActorClass, const int Quantity, TArray<UPooledComponent*>& PooledComponents)
//{
//	if (!ObjectPools.Contains(ActorClass) || Quantity <= 0) {
//		UE_LOG(LogTemp, Warning, TEXT("Invalid Class or quantity <= 0 provided. Aborting Pool spawning"));
//		return;
//	}
//	//UE_LOG(LogTemp, Display, TEXT("YUP WE SAVED"));
//
//
//
//	PooledComponents.Reserve(Quantity);
//	TArray<UPooledComponent*> AllPooledComponents = ObjectPools[ActorClass].PooledComponents;
//
//	int counter = 0;
//
//
//	for (UPooledComponent* PooledComponent : AllPooledComponents)
//	{
//		if (!PooledComponent) continue;
//
//		if (!PooledComponent->IsActorActive()) {
//			PooledComponents.Add(PooledComponent);
//			counter++;
//			if (counter >= Quantity) return;
//		}
//
//	}
//
//	// Returns all the available actors, even if couldn't reach the quantity requested
//	return;
//	
//}


AActor* UObjectPoolingSubsystem::SpawnFromPool(const TSubclassOf<AActor>& ActorClass, const FTransform SpawnTransform)
{
	UPooledComponent* PooledComponent = GetFirstAvailablePooledComponent(ActorClass);
	if (!PooledComponent) {
		UE_LOG(LogTemp, Warning, TEXT("Attempted to spawn from pool when unavailable"));
		return nullptr;
	}
	PooledComponent->GetOwner()->SetActorTransform(SpawnTransform);
	PooledComponent->SetActorActive();
	return PooledComponent->GetOwner();
}

TArray<AActor*> UObjectPoolingSubsystem::SpawnMultipleFromPool(const TSubclassOf<AActor>& ActorClass, const int Quantity, const FTransform SpawnTransform)
{
	//Trace for performance
	//TRACE_CPUPROFILER_EVENT_SCOPE(UObjectPoolingSubsystem::SpawnMultipleFromPool());

	TArray<AActor*> SpawnedActors;

	if (!ObjectPools.Contains(ActorClass) || Quantity <= 0) {
		UE_LOG(LogTemp, Warning, TEXT("Invalid Class or quantity <= 0 provided. Aborting Pool spawning"));
		return SpawnedActors;
	}


	SpawnedActors.Reserve(Quantity);
	TArray<UPooledComponent*> &PooledComponents = ObjectPools[ActorClass].PooledComponents;

	int counter = 0;


	for (UPooledComponent* PooledComponent : PooledComponents)
	{
		if (!PooledComponent) continue;

		if (!PooledComponent->IsActorActive()) {
			PooledComponent->GetOwner()->SetActorTransform(SpawnTransform);
			PooledComponent->SetActorActive();
			SpawnedActors.Add(PooledComponent->GetOwner());
			counter++;
			if (counter >= Quantity) return SpawnedActors;
		}

	}
	
	
	//Spawn Actors and add to both the pool and the array PooledComponents if the ones available in the pool were not enough
	if (PooledComponents.Num() < Quantity) {

		InitializePool(ActorClass, Quantity - PooledComponents.Num());
		SpawnedActors.Append(SpawnMultipleFromPool(ActorClass, Quantity - PooledComponents.Num(), SpawnTransform));
	}

	return SpawnedActors;
}

int UObjectPoolingSubsystem::GetNumberOfAvailableFromPool(const TSubclassOf<AActor>& ActorClass)
{
	if (!ObjectPools.Contains(ActorClass)) return 0;

	TArray<UPooledComponent*> &PooledComponents = ObjectPools[ActorClass].PooledComponents;

	int counter = 0;

	for (UPooledComponent* &PooledComponent : PooledComponents)
	{
		if (!PooledComponent) continue;

		if (!PooledComponent->IsActorActive()) {
			counter++;
		}

	}
	return counter;
}
