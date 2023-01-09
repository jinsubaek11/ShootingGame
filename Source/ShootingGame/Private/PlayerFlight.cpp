#include "PlayerFlight.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Enemy.h"
#include "UltimateBullet.h"
#include "AttackBarrier.h"
#include "EngineUtils.h"
#include "ObjectsPool.h"
#include "NormalBulletPool.h"
#include "SubBulletPool.h"
#include "PooledStrongBullet.h"
#include "StrongBulletPool.h"
#include "PooledObject.h"
#include "PooledEnemyBullet.h"
#include "Fence_Horizontal.h"
#include "Fence_Vertical.h"
#include "TengaiGameMode.h"
#include "MainWidget.h"
#include "Item.h"
#include "kismet/GameplayStatics.h"


APlayerFlight::APlayerFlight()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));
	boxComp->SetBoxExtent(FVector(50));
	//boxComp->SetSimulatePhysics(true);
	//boxComp->SetNotifyRigidBodyCollision(true);

	SetRootComponent(boxComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>cubemesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	if (cubemesh.Succeeded())
	{
		meshComp->SetStaticMesh(cubemesh.Object);
	}

	meshComp->SetRelativeLocation(FVector(0, 0, -50));
}

void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();
	
	velocity = FVector(0, -20, 30);
	gravity = FVector(0, 0, -9.8);

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &APlayerFlight::OnOverlap);
	boxComp->OnComponentHit.AddDynamic(this, &APlayerFlight::OnFenceHit);

	SetAttackBarrier(attackLevel);

	tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());

	normalBulletPool = GetWorld()->SpawnActor<ANormalBulletPool>();
	subBulletPool = GetWorld()->SpawnActor<ASubBulletPool>();
	strongBulletPool = GetWorld()->SpawnActor<AStrongBulletPool>();
	ultimate = GetWorld()->SpawnActor<AUltimateBullet>(GetActorLocation(), FRotator().ZeroRotator);
}

void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDead)
	{
		position = GetActorLocation();

		velocity += gravity * DeltaTime * 12;
		position += velocity * DeltaTime * 12;

		SetActorLocation(position, true);

		ultimateDurationTime = 0.f;
		isFireUltimate = false;
		ultimate->SetActive(false);
		
		return;
	}

	// 기본 이동
	float spd = tengaiGM->playSpeed;
	FVector newLoca = GetActorLocation();
	newLoca.Y = newLoca.Y + spd * DeltaTime;
	SetActorLocation(newLoca);
	direction.Normalize();
	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime, true);

	if (attackLevel > AttackLevel::STRONG) return;

	if (attackBarriers.Num() < (uint8)attackLevel)
	{
		SetAttackBarrier(attackLevel);
	}

	if (ultimateCount && isFireUltimate)
	{
		ultimateDurationTime += DeltaTime;

		if (ultimateDurationTime <= ultimateMaxDurationTime)
		{
			ultimate->SetActive(true);
			ultimate->SetActorLocation(GetActorLocation() + GetActorRightVector() * 1300);
		}
		else
		{
			ultimateDurationTime = 0.f;
			isFireUltimate = false;
			ultimate->SetActive(false);
		}

		return;
	}

	if (readyToSubAttack && isFireSubAttack && !isShooting)
	{
		strongBulletPool->SpawnPooledObject(GetActorLocation(), GetActorLocation() + GetActorRightVector());
		isFireSubAttack = false;
		readyToSubAttack = false;

		return;
	}

	if (subAttackWaitingTime >= subAttackCoolTime)
	{
		readyToSubAttack = true;

		return;
	}

	if (!isShooting) return;

	shootWaitingTime += DeltaTime;
	subAttackWaitingTime += DeltaTime;

	if (shootWaitingTime >= shootCoolTime)
	{
		if (attackLevel == AttackLevel::WEAK)
		{
			normalBulletPool->SpawnPooledObject(GetActorLocation(), GetActorLocation() + GetActorRightVector());
			UGameplayStatics::PlaySound2D(this, playerFire, 1.f, 1.f, 0.f);
		}
		else
		{
			for (int i = MIN_DEGREE * (uint8)attackLevel; i <= MAX_DEGREE * (uint8)attackLevel; i += COUNT_CONTROL_VAR / (uint8)attackLevel)
			{
				FVector playerLocation = GetActorLocation();
				FVector targetDirection = FVector(
					0,
					playerLocation.Y + FMath::Cos(FMath::DegreesToRadians(i)),
					playerLocation.Z + FMath::Sin(FMath::DegreesToRadians(i))
				);

				normalBulletPool->SpawnPooledObject(playerLocation, targetDirection);
			}
			UGameplayStatics::PlaySound2D(this, playerFire, 1.f, 1.f, 0.f);
		}
		
		enemies.Empty();

		for (TActorIterator<AEnemy> it(GetWorld()); it; ++it)
		{
			enemies.Emplace(*it);
		}

		for (AAttackBarrier* attackBarrier : attackBarriers)
		{
			if (enemies.Num() > 0)
			{
				attackBarrier->Shoot(enemies[FMath::RandRange(0, enemies.Num() - 1)]->GetActorLocation());
			}
			else
			{
				FVector randomDest = FVector(0, 2000, FMath::RandRange(-600, 600));
				attackBarrier->Shoot(randomDest);
			}
		}

		shootWaitingTime = 0.f;
	}
}

void APlayerFlight::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Horizontal", this, &APlayerFlight::HorizontalInput);
	PlayerInputComponent->BindAxis("Vertical", this, &APlayerFlight::VerticalInput);
	PlayerInputComponent->BindAxis("Fire", this, &APlayerFlight::Fire);
	PlayerInputComponent->BindAction("StrongFire", EInputEvent::IE_Released, this, &APlayerFlight::ShootStrongAttack);
	PlayerInputComponent->BindAction("Ultimate", EInputEvent::IE_Pressed, this, &APlayerFlight::ShootUltimate);
}

int32 APlayerFlight::GetLifeCount() const
{
	return lifeCount;
}

int32 APlayerFlight::GetMaxLifeCount() const
{
	return MAX_LIFE_COUNT;
}

int32 APlayerFlight::GetUltimateCount() const
{
	return ultimateCount;
}

int32 APlayerFlight::GetMaxUltimateCount() const
{
	return MAX_ULTIMATE_COUNT;
}

void APlayerFlight::AddUltimateCount()
{
	ultimateCount += 1;
}

bool APlayerFlight::GetIsDead() const
{
	return isDead;
}

bool APlayerFlight::GetIsInvincibility() const
{
	return isInvincibility;
}

void APlayerFlight::SetAttackLevel(AttackLevel level)
{
	if (level > AttackLevel::STRONG) return;
	
	attackLevel = level;
}

uint8 APlayerFlight::GetAttackLevel() const
{
	return (uint8)attackLevel;
}

ANormalBulletPool* APlayerFlight::GetNormalBulletPool()
{
	return normalBulletPool;
}

void APlayerFlight::SetAttackBarrier(AttackLevel level)
{
	if (level > AttackLevel::STRONG) return;

	if (attackBarriers.Num() > 0)
	{
		for (auto attackBarrier : attackBarriers)
		{
			attackBarrier->Destroy();
		}
	}
	
	attackBarriers.Empty();

	for (uint8 i = 1; i <= (uint8)level; i++)
	{
		FVector spawnLocation = FVector(
			0,
			GetActorLocation().Y + FMath::Sin(FMath::DegreesToRadians(120 * i)) * 150,
			GetActorLocation().Z + FMath::Cos(FMath::DegreesToRadians(120 * i)) * 150
		);

		AAttackBarrier* attackBarrier = GetWorld()->SpawnActor<AAttackBarrier>(spawnLocation, FRotator().ZeroRotator);
		attackBarrier->SetStartAngle(120 * i);
		attackBarriers.Add(attackBarrier);
	}	
}

void APlayerFlight::HorizontalInput(float value)
{
	if (!isDead)
	{
		direction.Y = value;
	}
}

void APlayerFlight::VerticalInput(float value)
{
	if (!isDead)
	{
		direction.Z = value;
	}
}

void APlayerFlight::Fire(float value)
{
	if (value >= 1.0f)
	{
		isShooting = true;
		//UGameplayStatics::PlaySound2D(this, playerFire, 1.f, 1.f, 0.f);
	}
	else
	{
		isShooting = false;
	}
}

void APlayerFlight::ShootStrongAttack()
{
	isFireSubAttack = true;
	subAttackWaitingTime = 0.f;
}

void APlayerFlight::Reset()
{
	SetActorLocation(FVector(0, -800, 0));
	velocity = FVector(0, -20, 30);
	gravity = FVector(0, 0, -9.8);

	ultimateCount = 1;
	ultimateDurationTime = 0.f;
	subAttackWaitingTime = 0.f;
	shootWaitingTime = 0.f;

	isShooting = false;
	readyToSubAttack = false;
	isFireSubAttack = false;
	isFireUltimate = false;
	isDead = false;

	isInvincibility = true;
	tengaiGM->mainUI->PrintUltimateCount(); 

	GetWorldTimerManager().SetTimer(timer, this, &APlayerFlight::SetFalseInvincibility, 3.f, false);
}

void APlayerFlight::SetFalseInvincibility()
{
	isInvincibility = false;
}

void APlayerFlight::ShootUltimate()
{
		isFireUltimate = true;
		ultimateCount --;
		tengaiGM->mainUI->PrintUltimateCount();
}

void APlayerFlight::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isInvincibility || isDead) return;

	APooledObject* enemyBullet = Cast<APooledObject>(OtherActor);

	if (enemyBullet)
	{
		enemyBullet->Reset();
		LifeCalculator();

		tengaiGM->mainUI->PrintLifeCount();
	}
}

void APlayerFlight::OnFenceHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (isInvincibility || !isDead) return;

	AFence_Horizontal* fenceH = Cast<AFence_Horizontal>(OtherActor);
	AFence_Vertical* fenceV = Cast<AFence_Vertical>(OtherActor);

	if ((fenceV || fenceH) && isDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
		Reset();
	}
}

void APlayerFlight::LifeCalculator()
{
	if (lifeCount > 1)
	{
		lifeCount -= 1;

		for (uint8 i = 1; i < (uint8)attackLevel; i++)
		{
			GetWorld()->SpawnActor<AItem>(powerItem, GetActorLocation() + GetActorUpVector() * 100 * i, FRotator::ZeroRotator);
		}
		// 궁극기 아이템 만들면 확인하고 스폰

		SetAttackLevel(AttackLevel::WEAK);
		SetAttackBarrier(AttackLevel::WEAK);

		isDead = true;

		UGameplayStatics::PlaySound2D(this, playerDown, 1.0f);
	}
	else
	{
		tengaiGM->ShowGameover();
		//Destroy();
	}
}