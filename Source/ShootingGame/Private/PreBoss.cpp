#include "PreBoss.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"
#include "EnemyBulletPool.h"
#include "EngineUtils.h"
#include "Boss.h"
#include "PlayerFlight.h"
#include "PooledObject.h"
#include "ItemWidget.h"
#include "HPWidget.h"
#include "kismet/GameplayStatics.h"
#include "TengaiGameMode.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"



APreBoss::APreBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	boxComp->SetCollisionProfileName(TEXT("BossPreset"));
	boxComp->SetBoxExtent(FVector(10, 180, 400));
	SetRootComponent(boxComp);

	spriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	spriteComp->SetSprite(LoadObject<UPaperSprite>(nullptr, TEXT("/ Script / Paper2D.PaperSprite'/Game/Asstets/BossEnemy/pre_boss.pre_boss'")));
	spriteComp->SetRelativeScale3D(FVector(1, 1, 1.5));
	spriteComp->SetRelativeRotation(FRotator(0, 90, 0));
	spriteComp->SetupAttachment(RootComponent);

	explosionFlipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("ExplosionFlipBook"));
	explosionFlipbookComp->SetFlipbook(LoadObject<UPaperFlipbook>(nullptr, TEXT("/Script/Paper2D.PaperFlipbook'/Game/Asstets/BossExplosion/boss_explosion_flip.boss_explosion_flip'")));
	explosionFlipbookComp->SetupAttachment(RootComponent);
	explosionFlipbookComp->SetRelativeLocation(FVector(-100, 0, 0));
	explosionFlipbookComp->SetRelativeRotation(FRotator(0, 90, 0));
	explosionFlipbookComp->SetRelativeScale3D(FVector(3, 1, 2.5));
	explosionFlipbookComp->SetHiddenInGame(true);
	explosionFlipbookComp->Stop();

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	widgetComp->SetPivot(FVector2D(-0.15, 0.2));
}

void APreBoss::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APreBoss::OnOverlap);
	enemyBulletPool = GetWorld()->SpawnActor<AEnemyBulletPool>();
	explosionPosition = TArray<FVector>({
		FVector(0), 
		FVector(0, -100, 100),
		FVector(0, 0, 100),
		FVector(0, 100, -100)
	});

	itemWidget = Cast<UItemWidget>(widgetComp->GetWidget());
	if (itemWidget)
	{
		itemWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void APreBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isPlayExplosionAnimation)
	{
		int32 playbackPosition = explosionFlipbookComp->GetPlaybackPositionInFrames();
		int32 filpbookLength = explosionFlipbookComp->GetFlipbookLengthInFrames();

		if (playbackPosition == filpbookLength - 1)
		{
			explosionFlipbookComp->Stop();
			explosionFlipbookComp->SetPlaybackPosition(0, true);
			explosionFlipbookComp->SetHiddenInGame(true);
			isPlayExplosionAnimation = false;

			UE_LOG(LogTemp, Warning, TEXT("explosion end"));
			explosionIndex++;
		}
	}

	if (isDead) return;

	shootWaitTime += DeltaTime;

	if (shootWaitTime >= shootCoolTime)
	{
		//int32 rand = FMath::RandRange(0, 1);
		int32 rand = 1;
		PreBossAttackType type = (PreBossAttackType)rand;

		APlayerFlight* player = Cast<APlayerFlight>(UGameplayStatics::GetPlayerPawn(this, 0));
		if (player)
		{
			targetLocation = player->GetActorLocation();
		}

		switch (type)
		{
		case PreBossAttackType::LINE_SHOOT:
			GetWorldTimerManager().SetTimer(shootTimer, this, &APreBoss::LineShoot, 0.1, true);
			break;
		case PreBossAttackType::RANDOM_SHOOT:
			GetWorldTimerManager().SetTimer(shootTimer, this, &APreBoss::RandomShoot, 0.1, true);
			break;
		}

		shootWaitTime = 0;
	}
}

void APreBoss::LineShoot()
{
	if (--lineShootCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(shootTimer);
		lineShootCallsRemaining = lineShootCallsMax;
		return;
	}

	enemyBulletPool->SpawnPooledObject(
		GetActorLocation() + FVector(0, -250, 80),
		targetLocation
	);
}

void APreBoss::RandomShoot()
{
	if (--randomShootCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(shootTimer);
		randomShootCallsRemaining = randomShootCallsMax;
		return;
	}

	int32 randomAngle = FMath::RandRange(0, 60);
	float randomY = FMath::Cos(FMath::DegreesToRadians(randomAngle + 180 - 30));
	float randomZ = FMath::Sin(FMath::DegreesToRadians(randomAngle + 180 - 30));
	FVector randomTarget = FVector(0, randomY, randomZ);

	enemyBulletPool->SpawnPooledObject(
		GetActorLocation() + FVector(0, -250, 80),
		GetActorLocation() + FVector(0, -250, 80) + randomTarget
	);
}

void APreBoss::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APooledObject* playerBullet = Cast<APooledObject>(OtherActor);
	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());

	if (playerBullet && tengaiGM)
	{
		hp -= playerBullet->GetAttackPower();
		playerBullet->Reset();

		if (!bossHPWidget)
		{
			bossHPWidget = tengaiGM->bossUI;
			return;
		}

		bossHPWidget->SetVisibility(ESlateVisibility::Visible);
		bossHPWidget->PrintCurrentHealth(hp, maxHP);
	}

	if (hp < 0)
	{
		SetActorEnableCollision(false);
		isDead = true;

		tengaiGM->AddScore(point);

		itemWidget->SetVisibility(ESlateVisibility::Visible);
		itemWidget->PrintMonsterScore(point);

		GetWorldTimerManager().SetTimer(explosionAnimationTimer, this, &APreBoss::PlayExplosionAnimation, 0.5f, true, 0);
		GetWorldTimerManager().SetTimer(destroyTimer, this, &APreBoss::DestroySelf, 2.5f, false);
	}
}

void APreBoss::PlayExplosionAnimation()
{
	if (--playExplosionAnimationRemaining < 0)
	{
		explosionFlipbookComp->SetHiddenInGame(true);
		explosionFlipbookComp->Stop();
		GetWorldTimerManager().ClearTimer(explosionAnimationTimer);
		return;
	}

	if (explosionIndex < explosionPosition.Num())
	{
		UE_LOG(LogTemp, Warning, TEXT("explosion"));
		explosionFlipbookComp->AddRelativeLocation(explosionPosition[explosionIndex]);
		explosionFlipbookComp->SetHiddenInGame(false);
		explosionFlipbookComp->Play();
		isPlayExplosionAnimation = true;
	}
}

void APreBoss::DestroySelf()
{
	GetWorld()->SpawnActor<ABoss>(boss, GetActorLocation() + FVector(0, 200, 200), FRotator::ZeroRotator);
	Destroy();
}
