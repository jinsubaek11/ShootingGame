#include "UltimateBullet.h"
#include "Enemy.h"
#include "components/BoxComponent.h"
#include "NiagaraComponent.h"

AUltimateBullet::AUltimateBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));
	boxComp->SetBoxExtent(FVector(70, 500, 85));
	SetRootComponent(boxComp);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Test/test.test'"));
	niagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	niagaraComp->SetRelativeLocation(FVector(0, -440, 0));
	niagaraComp->SetAsset(niagaraAsset.Object);
	niagaraComp->SetupAttachment(boxComp);
}

void AUltimateBullet::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AUltimateBullet::OnOverlap);
}

void AUltimateBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUltimateBullet::SetActive(bool isActive)
{
	if (isActive)
	{
		niagaraComp->SetActive(true);
	}
	else
	{
		niagaraComp->SetActive(false);
	}

	SetActorHiddenInGame(!isActive);
	SetActorEnableCollision(isActive);
	SetActorTickEnabled(isActive);
}

void AUltimateBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if (enemy != nullptr)
	{
		enemy->Destroy();
	}
}

