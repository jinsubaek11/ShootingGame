#include "PooledEnemyBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"


APooledEnemyBullet::APooledEnemyBullet()
{
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(25));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	boxComp->SetCollisionProfileName(TEXT("EnemyBulletPreset"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetStaticMesh(sphereMesh.Object);
	meshComp->SetWorldScale3D(FVector(0.2));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -10));

	speed = 1000.f;
}

void APooledEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APooledEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APooledEnemyBullet::Reset()
{
	SetDeactive();
}

void APooledEnemyBullet::SetActive(bool value)
{
	Super::SetActive(value);

	GetWorldTimerManager().SetTimer(
		lifeSpanTimer, this, &APooledEnemyBullet::SetDeactive, lifeSpan, false
	);
}

void APooledEnemyBullet::SetDeactive()
{
	Super::SetDeactive();
	OnPooledEnemyBulletDespawn.Broadcast(this);

}
