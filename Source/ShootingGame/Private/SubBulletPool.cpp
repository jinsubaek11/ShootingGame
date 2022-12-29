#include "SubBulletPool.h"
#include "PooledSubBullet.h"
#include "NormalBulletPool.h"


ASubBulletPool::ASubBulletPool()
{

}

void ASubBulletPool::BeginPlay()
{
	Super::BeginPlay();

	poolSize = 150;
	pooledObjectLifeSpan = 2.5f;

	UWorld* world = GetWorld();

	if (world)
	{
		for (uint16 i = 0; i < poolSize; ++i)
		{
			APooledSubBullet* bullet = world->SpawnActor<APooledSubBullet>(
				APooledSubBullet::StaticClass(), GetActorLocation(), FRotator().ZeroRotator
				);

			if (bullet)
			{
				bullet->SetActive(false);
				bullet->SetIndex(i);
				//bullet->SetLifeSpan(pooledObjectLifeSpan);
				bullet->OnPooledSubBulletDespawn.AddDynamic(
					this, &ASubBulletPool::OnPooledSubBulletDespawn
				);
				objectPool.Add(bullet);
			}
		}
	}
}

void ASubBulletPool::OnPooledSubBulletDespawn(APooledSubBullet* pooledSubBullet)
{
	spawnedPoolIndexes.Remove(pooledSubBullet->GetIndex());
	pooledSubBullet->ResetMovingTime();
	pooledSubBullet->SetActorLocation(GetActorLocation());
}
