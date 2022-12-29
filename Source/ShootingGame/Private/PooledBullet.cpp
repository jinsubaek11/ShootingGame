#include "PooledBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"


APooledBullet::APooledBullet()
{
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(25));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetStaticMesh(sphereMesh.Object);
	meshComp->SetWorldScale3D(FVector(0.2));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -10));

	speed = 1100.f;
}

void APooledBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APooledBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//void APooledBullet::SetDirection(FVector direction)
//{
//	bulletDirection = direction;
//}

void APooledBullet::Reset()
{
	SetDeactive();
}

void APooledBullet::SetActive(bool value)
{
	Super::SetActive(value);

	GetWorldTimerManager().SetTimer(
		lifeSpanTimer, this, &APooledBullet::SetDeactive, lifeSpan, false
	);
}

void APooledBullet::SetDeactive()
{
	Super::SetDeactive();
	OnPooledNormalBulletDespawn.Broadcast(this);
	//UE_LOG(LogTemp, Warning, TEXT("Pooled bullet deactive %d"), GetIndex());
}

float APooledBullet::GetAttackPower()
{
	return att;
}
