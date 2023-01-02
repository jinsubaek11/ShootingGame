#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectsPool.generated.h"

class APooledObject;

UCLASS()
class SHOOTINGGAME_API AObjectsPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectsPool();

protected:
	virtual void BeginPlay() override;

public:	
	APooledObject* SpawnPooledObject(FVector start, FVector end);

	TArray<APooledObject*> objectPool;

protected:	
	uint16 poolSize;
	float pooledObjectLifeSpan;

	TArray<uint16> spawnedPoolIndexes;
};
