#include "StrongBulletPool.h"
#include "PooledStrongBullet.h"

AStrongBulletPool::AStrongBulletPool()
{
}

void AStrongBulletPool::BeginPlay()
{
	Super::BeginPlay();

	poolSize = 3;
	pooledObjectLifeSpan = 2.5f;

	UWorld* world = GetWorld();

	if (world)
	{
		for (uint16 i = 0; i < poolSize; ++i)
		{
			APooledStrongBullet* bullet = world->SpawnActor<APooledStrongBullet>(
				APooledStrongBullet::StaticClass(), GetActorLocation(), FRotator().ZeroRotator
				);

			if (bullet)
			{
				bullet->SetActive(false);
				bullet->SetIndex(i);
				bullet->OnPooledStrongBulletDespawn.AddDynamic(
					this, &AStrongBulletPool::OnPooledStrongBulletDespawn
				);
				objectPool.Add(bullet);
			}
		}
	}
}

void AStrongBulletPool::OnPooledStrongBulletDespawn(APooledStrongBullet* pooledStrongBullet)
{
	spawnedPoolIndexes.Remove(pooledStrongBullet->GetIndex());
	pooledStrongBullet->SetActorLocation(GetActorLocation());
}
