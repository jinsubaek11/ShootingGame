#include "UltimateBullet.h"
#include "Enemy.h"
#include "components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "TengaiGameMode.h"

AUltimateBullet::AUltimateBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetCollisionProfileName(TEXT("BulletPreset"));
	boxComp->SetBoxExtent(FVector(70, 1200, 85));
	SetRootComponent(boxComp);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Test/test.test'"));
	niagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	niagaraComp->SetRelativeLocation(FVector(0, -1170, 0));
	niagaraComp->SetAsset(niagaraAsset.Object);
	niagaraComp->SetupAttachment(boxComp);
}

void AUltimateBullet::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(0, 3000, 0));
	niagaraComp->SetActive(false);
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AUltimateBullet::OnOverlap);
}

void AUltimateBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AUltimateBullet::SetActive(bool isActive)
{
	isUltimateActive = isActive;

	if (isUltimateActive)
	{
		niagaraComp->SetActive(true);
	}
	else
	{
		niagaraComp->SetActive(false);
	}

	SetActorHiddenInGame(!isUltimateActive);
	SetActorEnableCollision(isUltimateActive);
	SetActorTickEnabled(isUltimateActive);
}

void AUltimateBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* enemy = Cast<AEnemy>(OtherActor);

	if (enemy != nullptr)
	{
		ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
		if (tengaiGM)
		{
			tengaiGM->AddScore(enemy->point);
		}

		enemy->Destroy();
	}
}

