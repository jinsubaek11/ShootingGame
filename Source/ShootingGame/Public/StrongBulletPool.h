#pragma once

#include "CoreMinimal.h"
#include "ObjectsPool.h"
#include "StrongBulletPool.generated.h"


UCLASS()
class SHOOTINGGAME_API AStrongBulletPool : public AObjectsPool
{
	GENERATED_BODY()
	
public:
	AStrongBulletPool();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnPooledStrongBulletDespawn(class APooledStrongBullet* pooledStrongBullet);

};
