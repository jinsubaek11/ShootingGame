#pragma once

#include "CoreMinimal.h"
#include "ObjectsPool.h"
#include "SubBulletPool.generated.h"


UCLASS()
class SHOOTINGGAME_API ASubBulletPool : public AObjectsPool
{
	GENERATED_BODY()
	
public:
	ASubBulletPool();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnPooledSubBulletDespawn(class APooledSubBullet* pooledSubBullet);
};
