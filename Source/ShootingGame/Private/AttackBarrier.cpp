#include "AttackBarrier.h"
#include "PooledSubBullet.h"
#include "SubBulletPool.h"
#include "PlayerFlight.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


AAttackBarrier::AAttackBarrier()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(20));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	barrierSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	barrierSpriteComp->SetSprite(LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/Asstets/Bullet/human_cookie_Sprite.human_cookie_Sprite'")));
	barrierSpriteComp->SetWorldScale3D(FVector(0.2));
	barrierSpriteComp->SetRelativeRotation(FRotator(0, 90, 0));
	barrierSpriteComp->SetupAttachment(RootComponent);
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

void AAttackBarrier::Shoot(FVector target)
{
	APooledSubBullet* subBullet = Cast<APooledSubBullet>(player->subBulletPool->SpawnPooledObject(GetActorLocation(), target));
}

void AAttackBarrier::SetStartAngle(float degree)
{
	float radian = FMath::DegreesToRadians(degree);
	angle = radian;
}


