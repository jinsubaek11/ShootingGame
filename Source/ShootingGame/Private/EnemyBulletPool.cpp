#include "EnemyBulletPool.h"
#include "PooledEnemyBullet.h"


AEnemyBulletPool::AEnemyBulletPool()
{
}

void AEnemyBulletPool::BeginPlay()
{
	Super::BeginPlay();

	poolSize = 500;
	pooledObjectLifeSpan = 2.f;

	UWorld* world = GetWorld();

	if (world)
	{
		for (uint16 i = 0; i < poolSize; ++i)
		{
			APooledEnemyBullet* bullet = world->SpawnActor<APooledEnemyBullet>(
				APooledEnemyBullet::StaticClass(), GetActorLocation(), FRotator().ZeroRotator
				);

			if (bullet)
			{
				bullet->SetActive(false);
				bullet->SetIndex(i);
				bullet->OnPooledEnemyBulletDespawn.AddDynamic(
					this, &AEnemyBulletPool::OnPooledEnemyBulletDespawn
				);
				objectPool.Add(bullet);
			}
		}
	}
}

void AEnemyBulletPool::OnPooledEnemyBulletDespawn(APooledEnemyBullet* pooledEnemyBullet)
{
	spawnedPoolIndexes.Remove(pooledEnemyBullet->GetIndex());
	pooledEnemyBullet->SetActorLocation(GetActorLocation());
}
