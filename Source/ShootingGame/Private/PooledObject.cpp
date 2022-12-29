#include "PooledObject.h"

APooledObject::APooledObject()
{
	PrimaryActorTick.bCanEverTick = true;
}

void APooledObject::BeginPlay()
{
	Super::BeginPlay();	
}

void APooledObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + direction * speed * DeltaTime);
}

void APooledObject::Reset()
{
}

void APooledObject::SetActive(bool value)
{
	isActive = value;
	SetActorHiddenInGame(!isActive);
	SetActorEnableCollision(isActive);
	SetActorTickEnabled(isActive);
	//SetActorHiddenInGame(!isActive);

	if (isActive)
	{
		GetWorldTimerManager().SetTimer(
			lifeSpanTimer, this, &APooledObject::SetDeactive, lifeSpan, false
		);
	}
}

void APooledObject::SetDeactive()
{
	SetActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
}

void APooledObject::SetDirection(FVector start, FVector end)
{
	FVector dir = end - start;
	dir.Normalize();

	direction = dir;
}

void APooledObject::SetLifeSpan(float lifeTime)
{
	lifeSpan = lifeTime;
}

void APooledObject::SetIndex(uint16 objectIndex)
{
	index = objectIndex;
}

bool APooledObject::IsActive() const
{
	return isActive;
}

uint16 APooledObject::GetIndex() const
{
	return index;
}

float APooledObject::GetAttackPower()
{
	return 0;
}
