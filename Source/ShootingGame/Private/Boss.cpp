#include "Boss.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBulletPool.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	movementComp = CreateDefaultSubobject<UEnemyMovement>(TEXT("Movement"));
	AddOwnedComponent(movementComp);
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	SetMovingPath(30);

	enemyBulletPool = GetWorld()->SpawnActor<AEnemyBulletPool>();
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	movementComp->Custom(customPath, timeLine, 60, time);
	time += DeltaTime;

	uint16 currentTimeLineIndex = movementComp->GetCurrentTimeLineIndex();

	if (customPath[currentTimeLineIndex].type == MovingType::NONE)
	{
		if (!isFired)
		{
			Shoot(SelectAttackType());
			isFired = true;
		}
	}
	else
	{
		isFired = false;
	}
}

void ABoss::SetMovingPath(uint16 pathCount)
{
	for (int i = 0; i < pathCount; i++)
	{
		if (i == 0)
		{
			FMovement start;
			start.type = MovingType::LINEAR;
			//start.type = MovingType::NONE;
			start.position = GetActorLocation();
			customPath.Emplace(start);

			continue;
		}

		if (i == pathCount - 1)
		{
			if (customPath[i - 1].type == MovingType::NONE)
			{
				customPath[i - 1].type = MovingType::LINEAR;
			}

			FMovement end;
			end.type = MovingType::NONE;
			end.position = GetActorLocation();
			customPath.Emplace(end);

			continue;
		}

		uint8 randomIdx = FMath::RandRange(0, 3);
		float posY = FMath::RandRange(-200, 800);
		float posZ = FMath::RandRange(-500, 500);

		FMovement movement;
		//movement.type = MovingType::NONE;
		movement.type = (MovingType)randomIdx;
		movement.position = FVector(0, posY, posZ);

		if (randomIdx > (uint8)MovingType::NONE)
		{
			movement.type = MovingType::NONE;
		}

		if (MovingType::CURVE == (MovingType)randomIdx)
		{
			for (int j = 0; j < 2; j++)
			{
				float controlPosY = FMath::RandRange(-1000, 1000);
				float controlPosZ = FMath::RandRange(-700, 700);

				movement.controlPoints[j] = FVector(0, controlPosY, controlPosZ);
			}
		}

		if (customPath[i - 1].type == MovingType::NONE)
		{
			movement.position = customPath[i - 1].position;
		}

		customPath.Emplace(movement);
	}

	SetMovingTimeLine();
}

void ABoss::SetMovingTimeLine()
{
	for (int i = 0; i <= customPath.Num(); i++)
	{
		timeLine.Emplace(i / (float)customPath.Num());
	}
}

void ABoss::FanShoot()
{
	if (--fanShootCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		fanShootCallsRemaining = 10;
		return;
	}

	for (int j = 110; j <= 250; j += 20)
	{
		FVector BossLocation = GetActorLocation();
		FVector targetDirection = FVector(
			0,
			FMath::Cos(FMath::DegreesToRadians(j)),
			FMath::Sin(FMath::DegreesToRadians(j))
		);

		enemyBulletPool->SpawnPooledObject(
			BossLocation, 
			BossLocation + targetDirection
		);
	}
}

void ABoss::SequenceShoot()
{
	if (--sequenceShootCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		sequenceShootCallsRemaining = 60; 
		return;
	}

	float theta = -FMath::Cos((60 - sequenceShootCallsRemaining) / (float)60 * 3 * PI) * 40 + 180;
	FVector BossLocation = GetActorLocation();
	FVector targetDirection = FVector(
		0,
		FMath::Cos(FMath::DegreesToRadians(theta)),
		FMath::Sin(FMath::DegreesToRadians(theta))
	);

	enemyBulletPool->SpawnPooledObject(
		BossLocation,
		BossLocation + targetDirection
	);
}

void ABoss::SequenceSpiralShoot()
{
	if (--sequenceSpiralShootCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		sequenceSpiralShootCallsRemaining = 180;
		return;
	}

	float theta = (180 - sequenceSpiralShootCallsRemaining) / (float)180 * 10 * PI;
	FVector BossLocation = GetActorLocation();
	FVector targetDirection = FVector(
		0,
		FMath::Cos(theta),
		FMath::Sin(theta)
	);

	enemyBulletPool->SpawnPooledObject(
		BossLocation,
		BossLocation + targetDirection
	);
}

void ABoss::SpiralExplosion()
{
	if (--spiralExplosionCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		spiralExplosionCallsRemaining = 10;
		return;
	}

	for (int degree = 0 + spiralExplosionCallsRemaining * 13;	
		degree <= 360 + spiralExplosionCallsRemaining * 13; 
		degree += 15)
	{
		FVector BossLocation = GetActorLocation();
		FVector targetDirection = FVector(
			0,
			FMath::Cos(FMath::DegreesToRadians(degree)),
			FMath::Sin(FMath::DegreesToRadians(degree))
		);

		enemyBulletPool->SpawnPooledObject(
			BossLocation,
			BossLocation + targetDirection
		);
	}
}

void ABoss::RadialExplosion()
{
	if (--radialExplosionCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		radialExplosionCallsRemaining = 10;
		return;
	}

	for (int degree = 0; degree <= 360; degree += 20)
	{
		FVector BossLocation = GetActorLocation();
		FVector targetDirection = FVector(
			0,
			FMath::Cos(FMath::DegreesToRadians(degree)),
			FMath::Sin(FMath::DegreesToRadians(degree))
		);

		enemyBulletPool->SpawnPooledObject(
			BossLocation,
			BossLocation + targetDirection
		);
	}
}

AttackType ABoss::SelectAttackType()
{
	uint8 randomIdx = FMath::RandRange(0, 4);

	return (AttackType)randomIdx;
	//return AttackType::SPIRAL_EXPLOSION;
}

void ABoss::Shoot(AttackType attackType)
{
	switch (attackType)
	{
	case AttackType::FAN_SHOOT:
		GetWorldTimerManager().SetTimer(timer, this, &ABoss::FanShoot, 0.1, true);
		break;
	case AttackType::SEQUENCE_SHOOT:
		GetWorldTimerManager().SetTimer(timer, this, &ABoss::SequenceShoot, 0.05, true);
		break;
	case AttackType::SEQUENCE_SPIRAL_SHOOT:
		GetWorldTimerManager().SetTimer(timer, this, &ABoss::SequenceSpiralShoot, 0.015, true);
		break;
	case AttackType::RADIAL_EXPLOSION:
		GetWorldTimerManager().SetTimer(timer, this, &ABoss::RadialExplosion, 0.1, true);
		break;
	case AttackType::SPIRAL_EXPLOSION:
		GetWorldTimerManager().SetTimer(timer, this, &ABoss::SpiralExplosion, 0.07, true);
		break;
	default:
		break;
	}
}

