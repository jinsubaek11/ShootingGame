#include "PooledStrongBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "kismet/GameplayStatics.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Enemy.h"
#include "Boss.h"
#include "MidBoss.h"
#include "PreBoss.h"
#include "EngineUtils.h"
#include "PlayerFlight.h"
#include "NormalBulletPool.h"

APooledStrongBullet::APooledStrongBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(50));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	runFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RunFlipBook"));
	runFlipbookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/Bullet/strong_bullet/Curby_Move.Curby_Move'")));
	runFlipbookComp->SetRelativeRotation(FRotator(0, 90, 0));
	runFlipbookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	runFlipbookComp->SetupAttachment(RootComponent);

	stopFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("StopFlipBook"));
	stopFlipbookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/Bullet/strong_bullet/Curby_Stop.Curby_Stop'")));
	stopFlipbookComp->SetRelativeRotation(FRotator(0, 90, 0));
	stopFlipbookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	stopFlipbookComp->SetupAttachment(RootComponent);
	stopFlipbookComp->SetHiddenInGame(true);

	rotateFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("RotateFlipBook"));
	rotateFlipbookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/Bullet/strong_bullet/Curby_Rotate.Curby_Rotate'")));
	rotateFlipbookComp->SetRelativeRotation(FRotator(0, 90, 0));
	rotateFlipbookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	rotateFlipbookComp->SetupAttachment(RootComponent);
	rotateFlipbookComp->SetHiddenInGame(true);

	speed = 1000.f;
}

void APooledStrongBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APooledStrongBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isFired) return;

	for (TActorIterator<AEnemy> it(GetWorld()); it; ++it)
	{
		if (it)
		{
			enemy = *it;
			targetLocation = enemy->GetActorLocation();
			break;
		}
	}

	if (!enemy)
	{
		for (TActorIterator<AMidBoss> it(GetWorld()); it; ++it)
		{
			if (it)
			{
				midBoss = *it;
				targetLocation = midBoss->GetActorLocation();
				break;
			}
		}
	}

	if (!enemy && !midBoss)
	{
		for (TActorIterator<APreBoss> it(GetWorld()); it; ++it)
		{
			if (it)
			{
				preBoss = *it;
				targetLocation = preBoss->GetActorLocation();
				break;
			}
		}
	}

	if (!enemy && !midBoss && !preBoss)
	{
		for (TActorIterator<ABoss> it(GetWorld()); it; ++it)
		{
			if (it)
			{
				boss = *it;
				targetLocation = boss->GetActorLocation();
				break;
			}
		}
	}

	if (enemy || midBoss || preBoss || boss)
	{		
		float distance = (GetActorLocation() - targetLocation).Length();

		if (distance < 250)
		{
			direction = FVector(0);
			SetAnimation(StrongBulletAnimationType::STOP);
			isFired = true;
			GetWorldTimerManager().SetTimer(timer, this, &APooledStrongBullet::SetIsFiredFalse, 1.f, false);
			GetWorldTimerManager().SetTimer(shootTimer, this, &APooledStrongBullet::ShootStrongSubBullet, 0.15f, true);
			return;
		}

		direction = (targetLocation - GetActorLocation()).GetSafeNormal();
	}
	else
	{
		direction = FVector(0, 1, 0);
	}
}

void APooledStrongBullet::Reset()
{
	SetDeactive();
}

void APooledStrongBullet::SetActive(bool value)
{
	Super::SetActive(value);

	enemy = nullptr;
	midBoss = nullptr;
	boss = nullptr;
	isFired = false;
	SetAnimation(StrongBulletAnimationType::RUN);

	GetWorldTimerManager().SetTimer(
		lifeSpanTimer, this, &APooledStrongBullet::SetDeactive, lifeSpan, false
	);
}

void APooledStrongBullet::SetDeactive()
{
	Super::SetDeactive();

	OnPooledStrongBulletDespawn.Broadcast(this);
}

float APooledStrongBullet::GetAttackPower()
{
	return att;
}

void APooledStrongBullet::SetAnimation(StrongBulletAnimationType type)
{
	switch (type)
	{
	case StrongBulletAnimationType::RUN:
		runFlipbookComp->SetHiddenInGame(false);
		stopFlipbookComp->SetHiddenInGame(true);
		rotateFlipbookComp->SetHiddenInGame(true);
		direction = (targetLocation - GetActorLocation()).GetSafeNormal();
		break;

	case StrongBulletAnimationType::STOP:
		runFlipbookComp->SetHiddenInGame(true);
		stopFlipbookComp->SetHiddenInGame(false);
		rotateFlipbookComp->SetHiddenInGame(true);
		direction = FVector(0);
		break;
	}
}

void APooledStrongBullet::ShootStrongSubBullet()
{
	if (--ShootStrongSubBulletRemaining < 0)
	{
		ShootStrongSubBulletRemaining = 3;
		GetWorldTimerManager().ClearTimer(shootTimer);
		return;
	}

	APlayerFlight* player = Cast<APlayerFlight>(UGameplayStatics::GetPlayerPawn(this, 0));

	float y = targetLocation.Y - GetActorLocation().Y;
	float z = targetLocation.Z - GetActorLocation().Z;

	float targetAngle = FMath::RadiansToDegrees(FMath::Atan2(z, y));
	
	for (int i = 0; i < 3; i++, targetAngle += 5)
	{
		float radY = FMath::Cos(FMath::DegreesToRadians(targetAngle - 5));
		float radZ = FMath::Sin(FMath::DegreesToRadians(targetAngle - 5));

		player->GetNormalBulletPool()->SpawnPooledObject(
			GetActorLocation(), GetActorLocation() + FVector(0, radY, radZ));
	}
}

void APooledStrongBullet::SetIsFiredFalse()
{
	SetDeactive();
}

