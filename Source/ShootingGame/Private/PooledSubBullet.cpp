#include "PooledSubBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"

APooledSubBullet::APooledSubBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(25));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> subBulletMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_QuadPyramid.Shape_QuadPyramid'"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetStaticMesh(subBulletMesh.Object);
	meshComp->SetWorldScale3D(FVector(0.2));
	meshComp->SetupAttachment(RootComponent);
	meshComp->SetRelativeLocation(FVector(0, 0, -10));
}

void APooledSubBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APooledSubBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	movingTime += DeltaTime / durationTime;
		
	if (isActive)
	{
		SetActorLocation(GetCalculateLocation(movingTime));
	}
}

void APooledSubBullet::Reset()
{
	ResetMovingTime();
	SetDeactive();
}

void APooledSubBullet::SetDirection(FVector start, FVector end)
{
	int16 y1 = FMath::RandRange(start.Y, start.Y + 1000);
	int16 y2 = FMath::RandRange(start.Y, start.Y + 1000);
	int16 z1 = FMath::RandRange(start.Z - 600, start.Z + 600);
	int16 z2 = FMath::RandRange(start.Z - 600, start.Z + 600);

	startPosition = start;
	endPosition = end;
	firstControlPosition = FVector(0, y1, z1);
	secondControlPosition = FVector(0, y2, z2);
}

void APooledSubBullet::ResetMovingTime()
{
	movingTime = 0;
}

void APooledSubBullet::SetActive(bool value)
{
	Super::SetActive(value);

	GetWorldTimerManager().SetTimer(
		lifeSpanTimer, this, &APooledSubBullet::SetDeactive, lifeSpan, false
	);
}

void APooledSubBullet::SetDeactive()
{
	Super::SetDeactive();

	OnPooledSubBulletDespawn.Broadcast(this);
}

FVector APooledSubBullet::GetCalculateLocation(float time) const
{
	float t = time;

	FVector position =
		(1 - t) * (1 - t) * (1 - t) * startPosition +
		3 * t * (1 - t) * (1 - t) * firstControlPosition +
		3 * t * t * (1 - t) * secondControlPosition +
		t * t * t * endPosition;
	
	return position;
}