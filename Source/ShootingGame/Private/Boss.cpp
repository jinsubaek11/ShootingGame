#include "Boss.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EnemyBulletPool.h"
#include "TengaiGameMode.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PooledObject.h"


ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 100, 150));
	boxComp->SetCollisionProfileName(TEXT("BossPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	movementComp = CreateDefaultSubobject<UEnemyMovement>(TEXT("Movement"));
	AddOwnedComponent(movementComp);

	attackFlipBookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("AttackFlipBook"));
	attackFlipBookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/BossEnemy/Boss_Attack1.Boss_Attack1'")));
	attackFlipBookComp->SetupAttachment(RootComponent);
	attackFlipBookComp->SetRelativeRotation(FRotator(0, 90, 0));
	attackFlipBookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	attackFlipBookComp->SetHiddenInGame(true);

	normalWalkFlipBookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("NormalWalkFlipBook"));
	normalWalkFlipBookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/BossEnemy/Boss_Walk.Boss_Walk'")));
	normalWalkFlipBookComp->SetupAttachment(RootComponent);
	normalWalkFlipBookComp->SetRelativeRotation(FRotator(0, 90, 0));
	normalWalkFlipBookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	normalWalkFlipBookComp->SetHiddenInGame(true);

	walkWithSwordFlipBookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("WalkWithSwordFlipBook"));
	walkWithSwordFlipBookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/BossEnemy/Boss_Walk_Attack.Boss_Walk_Attack'")));
	walkWithSwordFlipBookComp->SetupAttachment(RootComponent); 
	walkWithSwordFlipBookComp->SetRelativeRotation(FRotator(0, 90, 0));
	walkWithSwordFlipBookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	walkWithSwordFlipBookComp->SetHiddenInGame(true);
	
	deadFlipBookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("DeadFlipBook"));
	deadFlipBookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/BossEnemy/Boss_Dead.Boss_Dead'")));
	deadFlipBookComp->SetupAttachment(RootComponent);
	deadFlipBookComp->SetRelativeRotation(FRotator(0, 90, 0));
	deadFlipBookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	deadFlipBookComp->SetHiddenInGame(true);

	currentFlipBookComponent = walkWithSwordFlipBookComp;
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnOverlap);

	SetMovingPath(30);
	enemyBulletPool = GetWorld()->SpawnActor<AEnemyBulletPool>();
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hp <= 0) return;

	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(gm);
	if (tengaiGM)
	{
		float spd = tengaiGM->playSpeed;
		//FVector newLoca = GetActorLocation();
		//newLoca.Y = newLoca.Y + spd * DeltaTime;
		//SetActorLocation(newLoca);

		for (int i = 0; i < customPath.Num(); i++)
		{
			//customPath[i].position.Y += spd * DeltaTime;
		}
	}

	movementComp->Custom(customPath, timeLine, 60, time);
	time += DeltaTime;

	uint16 currentTimeLineIndex = movementComp->GetCurrentTimeLineIndex();

	if (customPath[currentTimeLineIndex].type == MovingType::NONE)
	{
		if (isFiredComplete)
		{
			SetAnimation(AnimationType::WALK_WITH_SWORD);
			return;
		}

		if (!isFired)
		{
			SetAnimation(AnimationType::ATTACK);
			Shoot(SelectAttackType());
			isFired = true;
		}
	}
	else
	{
		SetAnimation(AnimationType::WALK_WITH_SWORD);
		isFiredComplete = false;
		isFired = false;
	}
}

void ABoss::SetAnimation(AnimationType animationType)
{
	SetAnimationComponent(animationType);
}

void ABoss::SetAnimationComponent(AnimationType animationType)
{
	currentAnimationType = animationType;

	switch (animationType)
	{
	case AnimationType::WALK:
		currentFlipBookComponent->SetHiddenInGame(true);
		currentFlipBookComponent = normalWalkFlipBookComp;
		currentFlipBookComponent->SetHiddenInGame(false);
		break;

	case AnimationType::WALK_WITH_SWORD:
		currentFlipBookComponent->SetHiddenInGame(true);
		currentFlipBookComponent = walkWithSwordFlipBookComp;
		currentFlipBookComponent->SetHiddenInGame(false);
		//UE_LOG(LogTemp, Warning, TEXT("WALK_WITH_SWORD"));
		break;

	case AnimationType::ATTACK:
		currentFlipBookComponent->SetHiddenInGame(true);
		currentFlipBookComponent = attackFlipBookComp;
		currentFlipBookComponent->SetHiddenInGame(false);

		//UE_LOG(LogTemp, Warning, TEXT("ATTACK"));
		break;

	case AnimationType::DEAD:
		currentFlipBookComponent->SetHiddenInGame(true);
		currentFlipBookComponent = deadFlipBookComp;
		currentFlipBookComponent->SetHiddenInGame(false);
		break;
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
		float posY = FMath::RandRange(GetActorLocation().Y - 800, GetActorLocation().Y);
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
				float controlPosY = FMath::RandRange(GetActorLocation().Y - 1000, GetActorLocation().Y + 300);
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
		isFiredComplete = true;
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
		isFiredComplete = true;
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
		isFiredComplete = true;
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
		isFiredComplete = true;
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
		isFiredComplete = true;
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

void ABoss::DestroySelf()
{
	Destroy();
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

void ABoss::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APooledObject* playerBullet = Cast<APooledObject>(OtherActor);

	if (playerBullet)
	{
		hp -= playerBullet->GetAttackPower();
		playerBullet->Reset();
	}

	if (hp < 0)
	{
		hp = 0;
		SetAnimation(AnimationType::DEAD);

		int32 playbackPosition = currentFlipBookComponent->GetPlaybackPositionInFrames();
		int32 filpbookLength = currentFlipBookComponent->GetFlipbookLengthInFrames();

		if (playbackPosition == filpbookLength - 1)
		{
			currentFlipBookComponent->Stop();
		}

		GetWorldTimerManager().SetTimer(timer, this, &ABoss::DestroySelf, 1.f, false);
	}
}

