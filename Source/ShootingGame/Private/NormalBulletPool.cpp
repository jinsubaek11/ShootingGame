#include "NormalBulletPool.h"
#include "PooledBullet.h"


ANormalBulletPool::ANormalBulletPool()
{

}

void ANormalBulletPool::BeginPlay()
{
	Super::BeginPlay();

	poolSize = 250;
	pooledObjectLifeSpan = 2.f;

	UWorld* world = GetWorld();

	if (world)
	{
		for (uint16 i = 0; i < poolSize; ++i)
		{
			APooledBullet* bullet = world->SpawnActor<APooledBullet>(
				APooledBullet::StaticClass(), GetActorLocation(), FRotator().ZeroRotator
				);

			if (bullet)
			{
				bullet->SetActive(false);
				bullet->SetIndex(i);
				bullet->OnPooledNormalBulletDespawn.AddDynamic(
					this, &ANormalBulletPool::OnPooledNormalBulletDespawn
				);
				objectPool.Add(bullet);
			}
		}
	}
}

void ANormalBulletPool::OnPooledNormalBulletDespawn(APooledBullet* pooledBullet)
{
	spawnedPoolIndexes.Remove(pooledBullet->GetIndex());
	pooledBullet->SetActorLocation(GetActorLocation());
}
