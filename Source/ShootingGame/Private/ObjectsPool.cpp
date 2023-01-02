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
			object->SetActorLocation(start);
			object->SetDirection(start, end);
			object->SetLifeSpan(pooledObjectLifeSpan);
			object->SetActive(true);
			spawnedPoolIndexes.Add(object->GetIndex());
			return object;
		}
	}

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





