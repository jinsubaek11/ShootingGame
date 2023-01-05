// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBullet.h"
#include "components/SphereComponent.h"
#include "components/StaticMeshComponent.h"
#include "PlayerFlight.h"
#include "EngineUtils.h"

// Sets default values
AEnemyBullet::AEnemyBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collision"));
	SetRootComponent(sphereComp);
	sphereComp->SetSphereRadius(30.0f);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
	
}

// Called when the game starts or when spawned
void AEnemyBullet::BeginPlay()
{
	Super::BeginPlay();

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
	if (enemyBulletTrace == 0)
	{
		direction = GetActorRightVector() * -1;
	}

	SetActorLocation(GetActorLocation() + direction * bulletSpeed * DeltaTime);
}
