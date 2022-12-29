#include "PooledStrongBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"

APooledStrongBullet::APooledStrongBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(50));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> sphereMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetStaticMesh(sphereMesh.Object);
	meshComp->SetWorldScale3D(FVector(0.5));
	meshComp->SetRelativeLocation(FVector(0, 0, -10));
	meshComp->SetupAttachment(RootComponent);

	speed = 700.f;
}

void APooledStrongBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APooledStrongBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APooledStrongBullet::Reset()
{
	SetDeactive();
}

void APooledStrongBullet::SetActive(bool value)
{
	Super::SetActive(value);

	GetWorldTimerManager().SetTimer(
		lifeSpanTimer, this, &APooledStrongBullet::SetDeactive, lifeSpan, false
	);
}

void APooledStrongBullet::SetDeactive()
{
	Super::SetDeactive();
	OnPooledStrongBulletDespawn.Broadcast(this);
}


