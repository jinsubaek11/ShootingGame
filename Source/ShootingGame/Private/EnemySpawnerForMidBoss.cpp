// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawnerForMidBoss.h"
#include "TengaiGameMode.h"

void AEnemySpawnerForMidBoss::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemySpawnerForMidBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	mainSpeed = tengaiGM->playSpeed;
	if (mainSpeed == 0 && !isSpawned)
	{
		GetWorld()->SpawnActor<AMidBoss>(midBossFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		isSpawned = true;
	}
}
