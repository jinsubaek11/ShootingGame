#include "PlayerFlight.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Bullet.h"
#include "StrongBullet.h"
#include "UltimateBullet.h"
#include "AttackBarrier.h"
#include "BulletPool.h"

APlayerFlight::APlayerFlight()
{
	PrimaryActorTick.bCanEverTick = true;

	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxcomp->SetBoxExtent(FVector(50));

	SetRootComponent(boxcomp);

	ConstructorHelpers::FObjectFinder<UStaticMesh>cubemesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	meshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshcomp->SetupAttachment(RootComponent);
	if (cubemesh.Succeeded())
	{
		meshcomp->SetStaticMesh(cubemesh.Object);
	}

	meshcomp->SetRelativeLocation(FVector(0, 0, -50));
}

void APlayerFlight::BeginPlay()
{
	Super::BeginPlay();
	
	SetAttackBarrier(attackLevel);
	bulletPool = GetWorld()->SpawnActor<ABulletPool>(FVector(0, 2000, 0), FRotator(0, 0, 0));
	ultimate = GetWorld()->SpawnActor<AUltimateBullet>(GetActorLocation(), FRotator().ZeroRotator);
	ultimate->SetActive(false);
}

void APlayerFlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	direction.Normalize();
	SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);

	if (attackLevel > (uint8)AttackLevel::STRONG) return;

	if (attackBarriers.Num() < attackLevel)
	{
		SetAttackBarrier(attackLevel);
	}

	if (ultimateCount && isFireUltimate)
	{
		ultimateDurationTime += DeltaTime;

		if (ultimateDurationTime <= ultimateMaxDurationTime)
		{
			ultimate->SetActive(true);
			ultimate->SetActorLocation(GetActorLocation() + GetActorRightVector() * 150);
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
		AStrongBullet* strongBullet = GetWorld()->SpawnActor<AStrongBullet>();
		strongBullet->SetActorLocation(GetActorLocation());
		strongBullet->SetDirection(GetActorRightVector());
		strongBullet->isActive = true;
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
		if (attackLevel == (uint8)AttackLevel::WEAK)
		{
			bulletPool->SpawnPooledBullet(GetActorLocation(), GetActorRightVector());
		}
		else
		{
			for (int i = MIN_DEGREE * attackLevel; i <= MAX_DEGREE * attackLevel; i += COUNT_CONTROL_VAR / attackLevel)
			{
				FVector playerLocation = GetActorLocation();
				FVector targetDirection = FVector(
					0,
					playerLocation.Y + FMath::Cos(FMath::DegreesToRadians(i)),
					playerLocation.Z + FMath::Sin(FMath::DegreesToRadians(i))
				);
				FVector bulletDirection = targetDirection - playerLocation;
				bulletDirection.Normalize();

				bulletPool->SpawnPooledBullet(playerLocation, bulletDirection);
			}
		}

		for (AAttackBarrier* attackBarrier : attackBarriers)
		{
			attackBarrier->Shoot();
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

void APlayerFlight::SetAttackLevel(uint8 level)
{
	if (level > (uint8)AttackLevel::STRONG) return;
	
	attackLevel = level;
}

uint8 APlayerFlight::GetAttackLevel() const
{
	return attackLevel;
}

ABulletPool* APlayerFlight::GetBulletPool() const
{
	return bulletPool;
}

void APlayerFlight::SetAttackBarrier(uint8 level)
{
	if (level > (uint8)AttackLevel::STRONG) return;

	if (attackBarriers.Num() > 0)
	{
		for (auto attackBarrier : attackBarriers)
		{
			attackBarrier->Destroy();
		}
	}
	
	attackBarriers.Empty();

	for (uint8 i = 1; i <= level; i++)
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
	//FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
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
		//readyToSubAttack = false;
		//subAttackWaitingTime = 0.f;
	}
}

void APlayerFlight::ShootStrongAttack()
{
	//UE_LOG(LogTemp, Warning, TEXT("ShootStrongAttack"));

	isFireSubAttack = true;
	subAttackWaitingTime = 0.f;
}

void APlayerFlight::ShootUltimate()
{
	isFireUltimate = true;
}
