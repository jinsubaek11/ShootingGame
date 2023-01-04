// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss.h"
#include "components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PlayerFlight.h"
#include "Fence_Vertical.h"
#include "Fence_Horizontal.h"
#include "PooledObject.h"
#include "EnemyBullet.h"

// Sets default values
AMidBoss::AMidBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(100));
	boxComp->SetCollisionProfileName("EnemyPreset");

	flipComp = CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook");
	flipComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMidBoss::BeginPlay()
{
	Super::BeginPlay();
	direction = GetActorUpVector() * -1;
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AMidBoss::OnOverlap);
}

// Called every frame
void AMidBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	currentTime += DeltaTime;

	// Z축으로 내려온다 (이동)
	if (currentTime <= 3)
	{
		SetActorLocation(GetActorLocation() + direction * DeltaTime * 200);
	}
	// 총알을 쏜다
	else if (currentTime >= 3 && currentTime <= 5)
	{
		if (!isShoot1)
		{
			for (int32 i = 0; i < 4; i++)
			{
				AEnemyBullet* bullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation() + FVector(0, 0, -1 * 150 / 2 * (4 - 1) + 150 * i), GetActorRotation());
				bullet->AddActorLocalRotation(FRotator(0, 0, -1 * 30 / 2 * (4 - 1) + 30 * i));
			}
			UE_LOG(LogTemp, Warning, TEXT("shoot1"));
			isShoot1 = true;
		}

	}
	else if (currentTime >= 5 && currentTime <= 7)
	{
		if (!isShoot2)
		{
			for (int32 i = 0; i < 4; i++)
			{
				AEnemyBullet* bullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation() + FVector(0, 0, -1 * 150 / 2 * (4 - 1) + 150 * i), GetActorRotation());
				bullet->AddActorLocalRotation(FRotator(0, 0, -1 * 30 / 2 * (4 - 1) + 30 * i));
			}
			UE_LOG(LogTemp,	Warning, TEXT("shoot2"));
			isShoot2 = true;
		}

	}
	else
	{
		SetActorLocation(GetActorLocation() + direction * DeltaTime * 200);
	}

// 	// 방향을 정하고
// 	if (!isSetDir && isSaved)
// 	{
// 		SetDirection();
// 		isSetDir = true;
// 	}
// 	// 3초간 빠르게 돌진한다
// 	if (isSetDir)
// 	{
// 		currentTime = 0;
// 		if (currentTime <= 3)
// 		{
// 			SetActorLocation(GetActorLocation() + direction * moveSpeed * DeltaTime);
// 		}
// 	}

	// 중앙 위치로 돌아온다

}


// void AMidBoss::SetDirection()
// {
// 	// 플레이어 방향으로
// 	for (TActorIterator<APlayerFlight> It(GetWorld()); It; ++It)
// 	{
// 		target = *It;
// 	}
// 	if (target != nullptr)
// 	{
// 		FVector targetDir = target->GetActorLocation() - GetActorLocation();
// 		targetDir.Normalize();
// 		direction = targetDir;
// 	}
// 
// }


void AMidBoss::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APooledObject* playerBullet = Cast<APooledObject>(OtherActor);

	if (playerBullet)
	{
		playerBullet->Reset();

		if (myHP > 0)
		{
			myHP -= playerBullet->GetAttackPower();
		}
		else
		{
			//DestroyEnemy();
			GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
			GetWorld()->SpawnActor<AItem>(itemFactoryUlti, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
		}

	}

	AFence_Vertical* fenceVer = Cast<AFence_Vertical>(OtherActor);
	if (fenceVer != nullptr)
	{
		direction.Y *= -1;
		return;
	}

	AFence_Horizontal* fenceHor = Cast<AFence_Horizontal>(OtherActor);
	if (fenceHor != nullptr)
	{
		direction.Z *= -1;
	}
}

// void AMidBoss::DestroyEnemy()
// {
// 	isDead = true;
// 
// 	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 	meshComp->SetHiddenInGame(true);
// 	flipbookComp->SetHiddenInGame(false);
// 	flipbookComp->Play();
// 
// 	GetWorldTimerManager().SetTimer(timer, this, &AEnemy::DestroySelf, 0.8f, false);
// }
