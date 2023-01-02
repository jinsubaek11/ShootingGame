#include "PlayerFlight.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Enemy.h"
#include "UltimateBullet.h"
#include "AttackBarrier.h"
#include "EngineUtils.h"
#include "TengaiGameMode.h"
#include "ObjectsPool.h"
#include "NormalBulletPool.h"
#include "SubBulletPool.h"
#include "PooledStrongBullet.h"
#include "StrongBulletPool.h"
#include "PooledObject.h"
#include "PooledEnemyBullet.h"
#include "Fence_Horizontal.h"



APlayerFlight::APlayerFlight()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetCollisionProfileName(TEXT("PlayerPreset"));
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetSimulatePhysics(true);
	boxComp->SetNotifyRigidBodyCollision(true);

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
		
		return;
	}

	// 기본 플레이 속도 적용
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(gm);
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
			ultimateCount--;
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

void APlayerFlight::SetAttackLevel(AttackLevel level)
{
	if (level > AttackLevel::STRONG) return;
	
	attackLevel = level;
}

uint8 APlayerFlight::GetAttackLevel()
{
	return (uint8)attackLevel;
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
	direction.Y = value;
}

void APlayerFlight::VerticalInput(float value)
{
	direction.Z = value;
}

void APlayerFlight::Fire(float value)
{
	if (value >= 1.0f)
	{
		isShooting = true;
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

	ultimateCount = 1;
	ultimateDurationTime = 0.f;
	shootWaitingTime = 0.f;
	subAttackWaitingTime = 0.f;
	isFireUltimate = false;
	isShooting = false;
	readyToSubAttack = false;
	isFireSubAttack = false;
}

void APlayerFlight::ShootUltimate()
{
	isFireUltimate = true;
}

void APlayerFlight::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APooledObject* enemyBullet = Cast<APooledObject>(OtherActor);

	if (enemyBullet)
	{
		enemyBullet->Reset();

		if (lifeCount > 0)
		{
			lifeCount -= 1;
			
			//isDead = true;
		}
		else
		{
			//Destroy();
		}

	}
	//UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	AFence_Horizontal* fence = Cast<AFence_Horizontal>(OtherActor);

	if (fence && isDead)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	}
}

void APlayerFlight::OnFenceHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	AFence_Horizontal* fence = Cast<AFence_Horizontal>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Hit"));

	if (fence && isDead)
	{
//		UE_LOG(LogTemp, Warning, TEXT("Reset"));
	}
}