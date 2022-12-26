#include "BulletPool.h"
#include "Bullet.h"
#include "PlayerFlight.h"
#include "Kismet/GameplayStatics.h"


ABulletPool::ABulletPool()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ABulletPool::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* world = GetWorld();
	
	if (world)
	{
		for (uint16 i = 0; i < poolSize; ++i)
		{
			ABullet* bullet = world->SpawnActor<ABullet>(
				ABullet::StaticClass(), GetActorLocation(), FRotator().ZeroRotator
			);

			if (bullet)
			{
				bullet->SetActive(false);
				bullet->SetIndex(i);
				bullet->OnPooledBulletDespawn.AddDynamic(
					this, &ABulletPool::OnPooledBulletDespawn
				);
				bulletPool.Add(bullet);
			}
		}
	}
}

ABullet* ABulletPool::SpawnPooledBullet(FVector location, FVector direction)
{
	for (ABullet* bullet : bulletPool)
	{
		if (bullet && !bullet->IsActive())
		{
			//UE_LOG(LogTemp, Warning, TEXT("Spawn"));
			bullet->SetActorLocation(location);
			bullet->SetDirection(direction);
			bullet->SetLifeSpan(pooledBulletLifeSpan);
			bullet->SetActive(true);
			spawnedPoolIndexes.Add(bullet->GetIndex());

			return bullet;
		}
	}

	if (bulletPool.Num() > 0)
	{
		uint16 targetIndex = spawnedPoolIndexes[0];

		spawnedPoolIndexes.Remove(targetIndex);
		ABullet* bullet = bulletPool[targetIndex];

		if (bullet)
		{
			bullet->SetActive(false);

			bullet->SetActorLocation(location);
			bullet->SetDirection(direction);
			bullet->SetLifeSpan(pooledBulletLifeSpan);

			bullet->SetActive(true);

			bulletPool.Add(bullet);

			return bullet;
		}
	}
	
	return nullptr;
}

void ABulletPool::OnPooledBulletDespawn(ABullet* bullet)
{
	spawnedPoolIndexes.Remove(bullet->GetIndex());
	bullet->SetActorLocation(GetActorLocation());
}



