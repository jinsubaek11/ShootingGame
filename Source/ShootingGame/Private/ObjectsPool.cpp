#include "ObjectsPool.h"
#include "PooledObject.h"

AObjectsPool::AObjectsPool()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AObjectsPool::BeginPlay()
{
	Super::BeginPlay();

}

APooledObject* AObjectsPool::SpawnPooledObject(FVector start, FVector end)
{
	for (APooledObject* object : objectPool)
	{
		if (object && !object->IsActive())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Spawn"));
			object->SetActorLocation(start);
			object->SetDirection(start, end);
			object->SetLifeSpan(pooledObjectLifeSpan);
			object->SetActive(true);
			spawnedPoolIndexes.Add(object->GetIndex());
			//UE_LOG(LogTemp, Warning, TEXT("%d"), spawnedPoolIndexes.Num());
			return object;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("%d"), spawnedPoolIndexes.Num());

	if (objectPool.Num() > 0 && spawnedPoolIndexes.Num() > 0)
	{
		uint16 targetIndex = spawnedPoolIndexes[0];

		spawnedPoolIndexes.Remove(targetIndex);
		APooledObject* object = objectPool[targetIndex];

		if (object)
		{
			object->SetActive(false);

			object->SetActorLocation(start);
			object->SetDirection(start, end);
			object->SetLifeSpan(pooledObjectLifeSpan);

			object->SetActive(true);

			objectPool.Add(object);

			return object;
		}
	}

	return nullptr;
}

//void AObjectsPool::OnPooledObjectDespawn(APooledObject* pooledObject)
//{
//	spawnedPoolIndexes.Remove(pooledObject->GetIndex());
//	pooledObject->SetActorLocation(GetActorLocation());
//}




