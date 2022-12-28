#pragma once

#include "CoreMinimal.h"
#include "ObjectsPool.h"
#include "NormalBulletPool.generated.h"


UCLASS()
class SHOOTINGGAME_API ANormalBulletPool : public AObjectsPool
{
	GENERATED_BODY()

public:
	ANormalBulletPool();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnPooledNormalBulletDespawn(class APooledBullet* pooledBullet);
};
