#include "Bullet.h"
#include "BulletPool.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "Enemy.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(25));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetStaticMesh(sphereMesh.Object);
	meshComp->SetWorldScale3D(FVector(0.2));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -10));
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();
}

void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isActive)
	{
		SetActorLocation(GetActorLocation() + bulletDirection * bulletSpeed * DeltaTime);
	}
}

void ABullet::SetActive(bool isActiveBullet)
{
	isActive = isActiveBullet;
	SetActorHiddenInGame(!isActive);

	if (isActive)
	{
		GetWorldTimerManager().SetTimer(
			lifeSpanTimer, this, &ABullet::SetDeactive, lifeSpan, false
		);
	}
}

void ABullet::SetDeactive()
{
	SetActive(false);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	OnPooledBulletDespawn.Broadcast(this);
}

void ABullet::SetLifeSpan(float lifeTime)
{
	lifeSpan = lifeTime;
}

void ABullet::SetDirection(FVector direction)
{
	bulletDirection = direction;
}

void ABullet::SetIndex(uint16 bulletIndex)
{
	index = bulletIndex;
}

bool ABullet::IsActive() const
{
	return isActive;
}

uint16 ABullet::GetIndex() const
{
	return index;
}


