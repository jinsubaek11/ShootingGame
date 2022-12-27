// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Enemy.h"
#include "components/SceneComponent.h"
#include "components/ArrowComponent.h"
#include "kismet/GameplayStatics.h"
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

	currentTime += DeltaTime;
	if (currentTime>=spawnTime)
	{
		GetWorld()->SpawnActor<AEnemy>(enemyFactory,GetActorLocation(),spawnArrow->GetComponentRotation());
		currentTime = 0;
	}

	// 기본 플레이 속도 적용
	AGameModeBase* gm = UGameplayStatics::GetGameMode(this);
	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(gm);
	float spd = tengaiGM->playSpeed;
	FVector newLoca = GetActorLocation();
	newLoca.Y = newLoca.Y + spd * DeltaTime;
	SetActorLocation(newLoca);
}

