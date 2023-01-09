// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "PlayerFlight.h"
#include "EngineUtils.h"
#include "TengaiGameMode.h"
#include "MainWidget.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(30.0f));
	boxComp->SetCollisionProfileName("EnemyBulletPreset");

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBullet::OnOverlap);

	if (enemyBulletTrace == 1)
	{
		for (TActorIterator<APlayerFlight> It(GetWorld()); It; ++It)
		{
			target = *It;
		}
		if (target != nullptr)
		{
			FVector targetDir = target->GetActorLocation() - GetActorLocation();
			targetDir.Normalize();
			direction = targetDir;
		}
	}

}

// Called every frame
void AEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	currentTime += DeltaTime;
	if (currentTime >= selfDestroyTime)
	{
		Destroy();
	}

	if (enemyBulletTrace == 0)
	{
		direction = GetActorRightVector() * -1;
	}
	SetActorLocation(GetActorLocation() + direction * bulletSpeed * DeltaTime);
}

void AEnemyBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Enemy Bullet Overlaped"));
	if (isInvincibility || isDead) return;
	
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);

	if (player != nullptr)
	{
		player->LifeCalculator();
		tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
		if (tengaiGM != nullptr)
		{
			//UE_LOG(LogTemp, Warning, TEXT("gm is ok"));
			if (tengaiGM->mainUI != nullptr)
			{
				//UE_LOG(LogTemp, Warning, TEXT("mainUI is ok"));
				tengaiGM->mainUI->PrintLifeCount();
			}
		}
		Destroy();
	}
}
