#pragma once

#include "CoreMinimal.h"
#include "ObjectsPool.h"
#include "EnemyBulletPool.generated.h"


UCLASS()
class SHOOTINGGAME_API AEnemyBulletPool : public AObjectsPool
{
	GENERATED_BODY()

public:
	AEnemyBulletPool();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnPooledEnemyBulletDespawn(class APooledEnemyBullet* pooledEnemyBullet);
};
