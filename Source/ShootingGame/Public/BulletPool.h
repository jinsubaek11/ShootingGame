// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletPool.generated.h"

UCLASS()
class SHOOTINGGAME_API ABulletPool : public AActor
{
	GENERATED_BODY()
	
public:	
	ABulletPool();

protected:
	virtual void BeginPlay() override;

public:
	class ABullet* SpawnPooledBullet(FVector location, FVector direction);
	UFUNCTION()
	void OnPooledBulletDespawn(class ABullet* bullet);

private:
	uint16 poolSize = 200;
	float pooledBulletLifeSpan = 2.5f;

	TArray<class ABullet*> bulletPool;
	TArray<uint16> spawnedPoolIndexes;
};


