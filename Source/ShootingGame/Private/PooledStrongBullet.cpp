#include "PooledStrongBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "Enemy.h"
#include "MidBoss.h"
#include "Boss.h"
#include "EngineUtils.h"

APooledStrongBullet::APooledStrongBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(50));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));

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

	speed = 700.f;
}

void APooledStrongBullet::BeginPlay()
{
	Super::BeginPlay();

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

	if (!midBoss)
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

	if (!boss)
	{

	}
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

float APooledStrongBullet::GetAttackPower()
{
	return att;
}

