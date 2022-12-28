#include "SubBullet.h"

ASubBullet::ASubBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh> subBulletMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_QuadPyramid.Shape_QuadPyramid'"));
	meshComp->SetStaticMesh(subBulletMesh.Object);
}


void ASubBullet::BeginPlay()
{
	Super::BeginPlay();
}

void ASubBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isActive)
	{
		accumulateTime += DeltaTime / durationTime;

		//float time = accumulateTime > durationTime ? 1.f : accumulateTime;

		//if (time >= 1.f) return;

		SetActorLocation(GetCalculateLocation(accumulateTime));
	}
}

void ASubBullet::SetDirection(FVector start, FVector end)
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

// cubic bezier
// (1 - t)^3 * p0 + 3t(1 - t)^2 * p1 + 3t^2 * (1 - t) * p2 + t^3 * p3
FVector ASubBullet::GetCalculateLocation(float time) const
{
	float t = time;
	FVector position = 
		(1 - t) * (1 - t) * (1 - t) * startPosition +
		3 * t * (1 - t) * (1 - t) * firstControlPosition +
		3 * t * t * (1 - t) * secondControlPosition + 
		t * t * t * endPosition;

	return position;
}
