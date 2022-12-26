#include "AttackBarrier.h"
#include "Bullet.h"
#include "SubBullet.h"
#include "BulletPool.h"
#include "PlayerFlight.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

AAttackBarrier::AAttackBarrier()
{
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

void AAttackBarrier::BeginPlay()
{
	Super::BeginPlay();
	player = Cast<APlayerFlight>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AAttackBarrier::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	accRotation += DeltaTime * 1.3f;

	float circlePositionY = 150.f * FMath::Cos(angle + accRotation);
	float circlePositionZ = 150.f * FMath::Sin(angle + accRotation);
	
	SetActorLocation(player->GetActorLocation() + FVector(0, circlePositionY, circlePositionZ));
}

void AAttackBarrier::Shoot()
{
	ASubBullet* bullet = GetWorld()->SpawnActor<ASubBullet>();
	bullet->SetDirection(GetActorLocation(), FVector(0, 600, 400));
	bullet->isActive = true;
	//player->GetBulletPool()->SpawnPooledBullet(GetActorLocation(), GetActorRightVector());
	//player->GetB
}

void AAttackBarrier::SetStartAngle(float degree)
{
	float radian = FMath::DegreesToRadians(degree);
	angle = radian;
}


