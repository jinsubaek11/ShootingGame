// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "components/SceneComponent.h"
#include "components/ArrowComponent.h"
#include "TengaiGameMode.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Scene"));
	SetRootComponent(rootScene);
	spawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	spawnArrow->SetupAttachment(rootScene);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	currentTime += DeltaTime;
// 	if (currentTime>=spawnTime)
// 	{
// 		GetWorld()->SpawnActor<AEnemy>(enemyFactory,GetActorLocation(),spawnArrow->GetComponentRotation());
// 		currentTime = 0;
// 	}

	//curLoc = GetActorLocation();
	

	worldTime += DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("time is %d"), worldTime);


	if (worldTime >= spawnTimer1)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer1 = 99999;
	}
	else if (worldTime >= spawnTimer2)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer2 = 99999;
	}
	else if (worldTime >= spawnTimer3)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer3 = 99999;
	}
	else if (worldTime >= spawnTimer4)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer4 = 99999;
	}
	else if (worldTime >= spawnTimer5)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer5 = 99999;
	}
	else if (worldTime >= spawnTimer6)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer6 = 99999;
	}
	else if (worldTime >= spawnTimer7)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer7 = 99999;
	}
	else if (worldTime >= spawnTimer8)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnTimer8 = 99999;
	}

	auto tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	if (tengaiGM != nullptr)
	{
		float spd = tengaiGM->playSpeed;
		FVector newLoca = GetActorLocation();
		newLoca.Y = newLoca.Y + spd * DeltaTime;
		SetActorLocation(newLoca);
	}
}

