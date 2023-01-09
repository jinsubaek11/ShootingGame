#include "UltimateBullet.h"
#include "Enemy.h"
#include "components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "TengaiGameMode.h"
#include "EnemyBullet.h"
#include "Boss.h"
#include "PreBoss.h"
#include "MidBoss.h"
#include "PooledEnemyBullet.h"
#include "HPWidget.h"


AUltimateBullet::AUltimateBullet()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetCollisionProfileName(TEXT("UltimatePreset"));
	boxComp->SetBoxExtent(FVector(70, 2000, 200));
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
	boxComp->OnComponentEndOverlap.AddDynamic(this, &AUltimateBullet::OnEndOverlap);

	tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
}

void AUltimateBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!isUltimateActive) return;

	if (midBoss && isOverlappingMidBoss)
	{
		if (midBoss->hpWidget)
		{
			//UE_LOG(LogTemp, Warning, TEXT("MidBoss"));
			midBoss->myHP -= att;
			midBoss->hpWidget->SetVisibility(ESlateVisibility::Visible);
			midBoss->hpWidget->PrintCurrentHealth(midBoss->myHP, midBoss->maxHP);
		}

		if (midBoss->myHP <= 0)
		{

			midBoss->DestroyMidBoss();
		}
	}

	if (preBoss && isOverlappingPreBoss)
	{
		if (tengaiGM->bossUI)
		{
			preBoss->hp -= att;
			tengaiGM->bossUI->SetVisibility(ESlateVisibility::Visible);
			tengaiGM->bossUI->PrintCurrentHealth(preBoss->hp, preBoss->maxHP);
		}

		if (preBoss->hp <= 0)
		{
			preBoss->DestroyPreBoss();
		}
	}

	if (boss && isOverlappingBoss)
	{
		if (tengaiGM->bossUI)
		{
			boss->hp -= att;
			tengaiGM->bossUI->SetVisibility(ESlateVisibility::Visible);
			tengaiGM->bossUI->PrintCurrentHealth(boss->hp, boss->maxHP);
		}
		if (boss->hp <= 0)
		{
			boss->DestroyBoss();
		}
	}
}

void AUltimateBullet::SetActive(bool isActive)
{
	isUltimateActive = isActive;

	if (isUltimateActive)
	{
		niagaraComp->SetActive(true);
		//isOverlapping = true;
	}
	else
	{
		niagaraComp->SetActive(false);
		//isOverlapping = false;
	}

	SetActorHiddenInGame(!isUltimateActive);
	SetActorEnableCollision(isUltimateActive);
	//SetActorTickEnabled(isUltimateActive);
}

void AUltimateBullet::OnOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!isUltimateActive) return;

	AEnemy* enemy = Cast<AEnemy>(OtherActor);
	if (enemy != nullptr)
	{
		if (tengaiGM)
		{
			tengaiGM->AddScore(enemy->point);
		}

		enemy->Destroy();
	}

	AMidBoss* overlappedMidBoss = Cast<AMidBoss>(OtherActor);
	if (overlappedMidBoss)
	{
		isOverlappingMidBoss = true;
		midBoss = overlappedMidBoss;
	}

	APreBoss* overlappedPreBoss = Cast<APreBoss>(OtherActor);
	if (overlappedPreBoss)
	{
		isOverlappingPreBoss = true;
		preBoss = overlappedPreBoss;
	}

	ABoss* overlappedBoss = Cast<ABoss>(OtherActor);
	if (overlappedBoss)
	{
		isOverlappingBoss = true;
		boss = overlappedBoss;
	}

	APooledEnemyBullet* pooledEnemyBullet = Cast<APooledEnemyBullet>(OtherActor);

	if (pooledEnemyBullet)
	{
		pooledEnemyBullet->Reset();
	}

	AEnemyBullet* enemyBullet = Cast<AEnemyBullet>(OtherActor);

	if (enemyBullet)
	{
		enemyBullet->Destroy();
	}
}

void AUltimateBullet::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AMidBoss* overlappedMidBoss = Cast<AMidBoss>(OtherActor);
	if (overlappedMidBoss)
	{
		isOverlappingMidBoss = false;
		midBoss = nullptr;
	}

	APreBoss* overlappedPreBoss = Cast<APreBoss>(OtherActor);
	if (overlappedPreBoss)
	{
		isOverlappingPreBoss = false;
		preBoss = nullptr;
	}

	ABoss* overlappedBoss = Cast<ABoss>(OtherActor);
	if (overlappedBoss)
	{
		isOverlappingBoss = false;
		boss = nullptr;
	}
}