// Fill out your copyright notice in the Description page of Project Settings.

#include "AttackBarrier.h"
#include "Bullet.h"
#include "PlayerFlight.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AAttackBarrier::AAttackBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(20));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> cornMesh(TEXT("/Script/Engine.StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetStaticMesh(cornMesh.Object);
	meshComp->SetWorldScale3D(FVector(0.3));
	meshComp->SetRelativeLocation(FVector(0, 0, -10));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AAttackBarrier::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAttackBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	accTime += DeltaTime;
	accRotation += DeltaTime;
	
	APlayerFlight* player = Cast<APlayerFlight>(UGameplayStatics::GetPlayerPawn(this, 0));
	float circlePositionY = 200.f * FMath::Sin(accRotation * 1.3f);
	float circlePositionZ = 200.f * FMath::Cos(accRotation * 1.3f);
	
	SetActorLocation(player->GetActorLocation() + FVector(0, circlePositionY, circlePositionZ));

	if (accTime >= delay)
	{
		FVector spawnPosition = GetActorLocation();
		FRotator spawnRotation = FRotator(0, 0, 0);
		
		// 헤더에서 경로 지정한 총알을 로드 후 스폰
		UClass* LoadedBpAsset = subBulletBP.LoadSynchronous();
		ABullet* bullet = GetWorld()->SpawnActor<ABullet>(LoadedBpAsset, spawnPosition, spawnRotation);
		bullet->SetLifeSpan(3.0f);

		accTime = 0.f;
	}
}

