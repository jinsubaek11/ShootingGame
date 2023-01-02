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

	curLoc = GetActorLocation();


	if (curLoc.Y >= spawnY1)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY1 = 99999;
	}
	else if (curLoc.Y >= spawnY2)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY2 = 99999;
	}
	else if (curLoc.Y >= spawnY3)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY3 = 99999;
	}
	else if (curLoc.Y >= spawnY4)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY4 = 99999;
	}
	else if (curLoc.Y >= spawnY5)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY5 = 99999;
	}
	else if (curLoc.Y >= spawnY6)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY6 = 99999;
	}
	else if (curLoc.Y >= spawnY7)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY7 = 99999;
	}
	else if (curLoc.Y >= spawnY8)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory, GetActorLocation(), spawnArrow->GetComponentRotation());
		spawnY8 = 99999;
	}

	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(gm);
	float spd = tengaiGM->playSpeed;
	FVector newLoca = GetActorLocation();
	newLoca.Y = newLoca.Y + spd * DeltaTime;
	SetActorLocation(newLoca);
}

