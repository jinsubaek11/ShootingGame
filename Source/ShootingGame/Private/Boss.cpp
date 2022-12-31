#include "Boss.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

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
	SetMovingTimeLine();
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
			Shoot();
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
			start.position = GetActorLocation();
			customPath.Emplace(start);

			continue;
		}

		if (i == pathCount - 1)
		{
			FMovement end;
			end.type = MovingType::NONE;
			end.position = GetActorLocation();
			customPath.Emplace(end);

			continue;
		}

		uint8 randomIdx = FMath::RandRange(0, 2);
		float posY = FMath::RandRange(-800, 800);
		float posZ = FMath::RandRange(-500, 500);

		FMovement movement;
		movement.type = (MovingType)randomIdx;
		movement.position = FVector(0, posY, posZ);

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
}

void ABoss::SetMovingTimeLine()
{
	for (int i = 0; i <= customPath.Num(); i++)
	{
		timeLine.Emplace(i / (float)customPath.Num());
	}
}

void ABoss::Shoot()
{
	UE_LOG(LogTemp, Warning, TEXT("Shoot"));
}

