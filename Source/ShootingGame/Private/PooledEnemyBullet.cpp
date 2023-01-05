#include "PooledEnemyBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


APooledEnemyBullet::APooledEnemyBullet()
{
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(25));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	boxComp->SetCollisionProfileName(TEXT("EnemyBulletPreset"));
	
	bulletSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	bulletSpriteComp->SetSprite(LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/Asstets/Bullet/SP_BossBullet.SP_BossBullet'")));
	bulletSpriteComp->SetupAttachment(RootComponent);
	bulletSpriteComp->SetWorldScale3D(FVector(1.7));
	bulletSpriteComp->SetRelativeLocation(FVector(0, 0, -10));
	bulletSpriteComp->SetRelativeRotation(FRotator(0, 90, 0));

	speed = 1000.f;
}

void APooledEnemyBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APooledEnemyBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APooledEnemyBullet::Reset()
{
	SetDeactive();
}

void APooledEnemyBullet::SetActive(bool value)
{
	Super::SetActive(value);

	GetWorldTimerManager().SetTimer(
		lifeSpanTimer, this, &APooledEnemyBullet::SetDeactive, lifeSpan, false
	);
}

void APooledEnemyBullet::SetDeactive()
{
	Super::SetDeactive();
	OnPooledEnemyBulletDespawn.Broadcast(this);

}
