#include "PooledBullet.h"
#include "components/BoxComponent.h"
#include "components/MeshComponent.h"
#include "components/ArrowComponent.h"
#include "PaperSpriteComponent.h"
#include "PaperSprite.h"


APooledBullet::APooledBullet()
{
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(25));
	SetRootComponent(boxComp);

	arrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	arrowComp->SetupAttachment(RootComponent);

	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));

	bulletSpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	bulletSpriteComp->SetSprite(LoadObject<UPaperSprite>(nullptr, TEXT("/Script/Paper2D.PaperSprite'/Game/Asstets/Bullet/pink_donut_Sprite.pink_donut_Sprite'")));
	bulletSpriteComp->SetWorldScale3D(FVector(0.13));
	bulletSpriteComp->SetRelativeRotation(FRotator(0, 90, 0));
	bulletSpriteComp->SetupAttachment(RootComponent);

	speed = 1100.f;
}

void APooledBullet::BeginPlay()
{
	Super::BeginPlay();
}

void APooledBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APooledBullet::Reset()
{
	SetDeactive();
}

void APooledBullet::SetActive(bool value)
{
	Super::SetActive(value);

	GetWorldTimerManager().SetTimer(
		lifeSpanTimer, this, &APooledBullet::SetDeactive, lifeSpan, false
	);
}

void APooledBullet::SetDeactive()
{
	Super::SetDeactive();
	OnPooledNormalBulletDespawn.Broadcast(this);
}

float APooledBullet::GetAttackPower()
{
	return att;
}
