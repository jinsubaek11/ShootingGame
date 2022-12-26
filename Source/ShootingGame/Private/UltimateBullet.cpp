#include "UltimateBullet.h"
#include "components/BoxComponent.h"
#include "NiagaraComponent.h"

AUltimateBullet::AUltimateBullet()
{
	PrimaryActorTick.bCanEverTick = false;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/Test/test.test'"));
	niagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	niagaraComp->SetAsset(niagaraAsset.Object);
	niagaraComp->SetupAttachment(boxComp);
}

void AUltimateBullet::BeginPlay()
{
	Super::BeginPlay();
	
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

