#include "Boss.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/StaticMeshComponent.h"
#include "kismet/GameplayStatics.h"
#include "EnemyBulletPool.h"
#include "TengaiGameMode.h"
#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PooledObject.h"
#include "EngineUtils.h"
#include "PreBoss.h"
#include "ItemWidget.h"
#include "HPWidget.h"

ABoss::ABoss()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50, 70, 130));
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

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	widgetComp->SetPivot(FVector2D(-0.15, 0.2));
}

void ABoss::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ABoss::OnOverlap);

	itemWidget = Cast<UItemWidget>(widgetComp->GetWidget());
	if (itemWidget)
	{
		itemWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	if (!bossHPWidget)
	{
		bossHPWidget = tengaiGM->bossUI;
	}

	GetWorldTimerManager().SetTimer(hpTimer, this, &ABoss::RecoverHPBar, 0.02f, true, 0.f);

	startOrigin = GetActorLocation();
	attackStartOrigin = startOrigin - FVector(0, 600, 0);

	SetMovingPath(50);
	
	for (TActorIterator<APreBoss> it(GetWorld()); it; ++it)
	{
		if (it)
		{
			enemyBulletPool = it->enemyBulletPool;
		}
	}
}

void ABoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (hp <= 0) return;
	
	startAnimationPlayTime += DeltaTime;
	if (!isPlayedStartAnimation || startAnimationPlayTime <= startAnimationDuration)
	{
		movementComp->Start(startOrigin, attackStartOrigin, startAnimationPlayTime, 3.f);
		SetAnimation(AnimationType::WALK);

		isPlayedStartAnimation = true;
		return;
	}

	movementComp->Custom(customPath, timeLine, 120, time);
	time += DeltaTime;

	uint16 currentTimeLineIndex = movementComp->GetCurrentTimeLineIndex();
	if (currentTimeLineIndex == customPath.Num() - 1)
	{
		movementComp->SetCurrentTimeLineIndex(0);
		time = 0;
		return;
	}

	if (customPath[currentTimeLineIndex].type == MovingType::NONE)
	{
		if (customPath[currentTimeLineIndex - 1].type == MovingType::NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Change Moving Type"));
			customPath[currentTimeLineIndex].type = MovingType::LINEAR;
			return;
		}

		if (isFiredComplete)
		{
			UE_LOG(LogTemp, Warning, TEXT("1"));
			SetAnimation(AnimationType::WALK_WITH_SWORD);
			//return;
		}

		if (!isFired)
		{
			UE_LOG(LogTemp, Warning, TEXT("2"));
			SetAnimation(AnimationType::ATTACK);
			Shoot(SelectAttackType());
			isFired = true;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("3"));
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
			start.position = attackStartOrigin;
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
		float posY = FMath::RandRange(attackStartOrigin.Y - 1000, attackStartOrigin.Y);
		float posZ = FMath::RandRange(-400, 400);

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
		fanShootCallsRemaining = 7;
		isFiredComplete = true;
		return;
	}

	for (int j = 110; j <= 250; j += 30)
	{
		FVector BossLocation = GetActorLocation();
		FVector targetDirection = FVector(
			0,
			FMath::Cos(FMath::DegreesToRadians(j)),
			FMath::Sin(FMath::DegreesToRadians(j))
		);

		enemyBulletPool->SpawnPooledObject(
			BossLocation + targetDirection * 200,
			BossLocation + targetDirection * 201
		);
	}
}

void ABoss::SequenceShoot()
{
	if (--sequenceShootCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		sequenceShootCallsRemaining = sequenceShootCallsMax;
		isFiredComplete = true;
		return;
	}

	float theta = -FMath::Cos(
		(sequenceShootCallsMax - sequenceShootCallsRemaining) / 
		(float)sequenceShootCallsMax * 2 * PI) * 40 + 180;
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

void ABoss::RecursiveShoot(int32 count, const FVector& location)
{
	if (count >= 5)
	{
		isFiredComplete = true;
		return;
	};

	for (float i = 0; i < 360; i += 120)
	{
		float theta = FMath::DegreesToRadians(i);

		FVector targetDirection = FVector(
			0,
			FMath::Cos(theta),
			FMath::Sin(theta)
		);

		enemyBulletPool->SpawnPooledObject(
			location + targetDirection * 80,
			location + targetDirection * 81
		);

		RecursiveShoot(count + 1, location + targetDirection * 80);
	}
}

void ABoss::SpiralExplosion()
{
	if (--spiralExplosionCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		spiralExplosionCallsRemaining = 7;
		isFiredComplete = true;
		return;
	}

	float randSpeedScale = FMath::RandRange(0.5, 1.0);

	for (int degree = 0 + spiralExplosionCallsRemaining * 13;	
		degree <= 360 + spiralExplosionCallsRemaining * 13; 
		degree += 30)
	{
		FVector BossLocation = GetActorLocation();
		FVector targetDirection = FVector(
			0,
			FMath::Cos(FMath::DegreesToRadians(degree)),
			FMath::Sin(FMath::DegreesToRadians(degree))
		);

		enemyBulletPool->SpawnPooledObject(
			BossLocation + targetDirection * 200,
			BossLocation + targetDirection * 201,
			randSpeedScale
		);
	}
}

void ABoss::RadialExplosion()
{
	if (--radialExplosionCallsRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(timer);
		radialExplosionCallsRemaining = 7;
		isFiredComplete = true;
		return;
	}

	for (int degree = 0; degree <= 360; degree += 30)
	{
		FVector BossLocation = GetActorLocation();
		FVector targetDirection = FVector(
			0,
			FMath::Cos(FMath::DegreesToRadians(degree)),
			FMath::Sin(FMath::DegreesToRadians(degree))
		);

		enemyBulletPool->SpawnPooledObject(
			BossLocation + targetDirection * 200,
			BossLocation + targetDirection * 201
		);
	}
}

AttackType ABoss::SelectAttackType()
{
	uint8 randomIdx = FMath::RandRange(0, 4);

	return (AttackType)randomIdx;
	//return AttackType::RECURSIVE_SHOOT;
}

void ABoss::DestroySelf()
{
	endingUI = CreateWidget<UEndingWidget>(GetWorld(), endingWidget);
	if (endingUI != nullptr)
	{
		endingUI->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}

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
	case AttackType::RECURSIVE_SHOOT:
		//GetWorldTimerManager().SetTimer(timer, this, &ABoss::SequenceSpiralShoot, 0.015, true);
		RecursiveShoot(1, GetActorLocation());
		break;
	case AttackType::RADIAL_EXPLOSION:
		GetWorldTimerManager().SetTimer(timer, this, &ABoss::RadialExplosion, 0.1, true);
		break;
	case AttackType::SPIRAL_EXPLOSION:
		GetWorldTimerManager().SetTimer(timer, this, &ABoss::SpiralExplosion, 0.1, true);
		break;
	default:
		break;
	}
}

void ABoss::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (startAnimationPlayTime <= startAnimationDuration) return;

	APooledObject* playerBullet = Cast<APooledObject>(OtherActor);

	if (playerBullet)
	{
		hp -= playerBullet->GetAttackPower();
		playerBullet->Reset();
		bossHPWidget->PrintCurrentHealth(hp, maxHP, FLinearColor::Green);
	}

	if (hp < 0)
	{
		SetActorEnableCollision(false);

		ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
		if (tengaiGM)
		{
			tengaiGM->AddScore(point);
		}

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

void ABoss::RecoverHPBar()
{
	if (--hpRecoverRemaining < 0)
	{
		GetWorldTimerManager().ClearTimer(hpTimer);
		return;
	}

	bossHPWidget->PrintCurrentHealth(hpRecoverRemainingMax - hpRecoverRemaining, 100, FLinearColor::Green);
}

