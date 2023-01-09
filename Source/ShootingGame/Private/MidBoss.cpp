// Fill out your copyright notice in the Description page of Project Settings.


#include "MidBoss.h"
#include "components/BoxComponent.h"
#include "PaperFlipbookComponent.h"
#include "PlayerFlight.h"
#include "Fence_Vertical.h"
#include "Fence_Horizontal.h"
#include "PooledObject.h"
#include "EnemyBullet.h"
#include "kismet/GameplayStatics.h"
#include "TengaiGameMode.h"
#include "Item.h"
#include "ItemWidget.h"
#include "HPWidget.h"
#include "Components/WidgetComponent.h"
#include "WarningWidget.h"

// Sets default values
AMidBoss::AMidBoss()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>("Box Collision");
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(100));
	boxComp->SetCollisionProfileName("EnemyPreset");

	flipComp = CreateDefaultSubobject<UPaperFlipbookComponent>("Flipbook");
	flipComp->SetupAttachment(RootComponent);

	itemWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	itemWidgetComp->SetupAttachment(RootComponent);
	itemWidgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	itemWidgetComp->SetPivot(FVector2D(-0.15, 0.2));

	hpWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Widget Component"));
	hpWidgetComp->SetupAttachment(RootComponent);
	hpWidgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	hpWidgetComp->SetPivot(FVector2D(0.5, 5.0));
	hpWidgetComp->SetDrawSize(FVector2D(150, 16));
}

// Called when the game starts or when spawned
void AMidBoss::BeginPlay()
{
	Super::BeginPlay();
	direction = GetActorUpVector() * -1;
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AMidBoss::OnOverlap);
	UGameplayStatics::PlaySound2D(this, dragonSpawned, 1.2f, 1.0f, 0.5f);

	itemWidget = Cast<UItemWidget>(itemWidgetComp->GetWidget());
	if (itemWidget)
	{
		itemWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	hpWidget = Cast<UHPWidget>(hpWidgetComp->GetWidget());
	if (hpWidget)
	{
		hpWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called every frame
void AMidBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (isDead) 
	{
		SetActorLocation(GetActorLocation() + direction * 200 * DeltaTime);
		return;
	};

	currentTime += DeltaTime;
	currentLoc = GetActorLocation();

	// Z축으로 내려온다 (z가 0일때까지)
	if (currentTime < 2 )
	{
		if (currentLoc.Z >= 0)
		{
			SetActorLocation(GetActorLocation() + direction * DeltaTime * 400);
		}
	}
	// 현재 위치를 저장한다
	else if (currentTime >= 2 && currentTime < 2.5)
	{
		mainLoc = GetActorLocation();
	}
	// 총알을 쏜다
	else if (currentTime >= 2.5 && currentTime < 3)
	{
		if (!isShoot1)
		{
			for (int32 i = 0; i < 5; i++)
			{
				AEnemyBullet* bullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation() + FVector(0, 0, -1 * 150 / 2 * (4 - 1) + 150 * i), GetActorRotation());
				bullet->AddActorLocalRotation(FRotator(0, 0, -1 * 30 / 2 * (5 - 1) + 30 * i));
			}
			isShoot1 = true;
		}

	}
	else if (currentTime >= 3 && currentTime < 3.5)
	{
		if (!isShoot2)
		{
			for (int32 i = 0; i < 6; i++)
			{
				AEnemyBullet* bullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation() + FVector(0, 0, -1 * 150 / 2 * (4 - 1) + 150 * i), GetActorRotation());
				bullet->AddActorLocalRotation(FRotator(0, 0, -1 * 30 / 2 * (6 - 1) + 30 * i));
			}
			isShoot2 = true;
		}

	}
	else if (currentTime >= 3.5 && currentTime < 4)
	{
		if (!isShoot3)
		{
			for (int32 i = 0; i < 7; i++)
			{
				AEnemyBullet* bullet = GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation() + FVector(0, 0, -1 * 150 / 2 * (4 - 1) + 150 * i), GetActorRotation());
				bullet->AddActorLocalRotation(FRotator(0, 0, -1 * 30 / 2 * (7 - 1) + 30 * i));
			}
			isShoot3 = true;
		}

	}
	// 플레이어쪽으로 방향을 정한다
	else if (currentTime >= 4 && currentTime < 4.5)
	{
		AActor* target = UGameplayStatics::GetActorOfClass(GetWorld(), APlayerFlight::StaticClass());
		directionPlayer = target->GetActorLocation() - GetActorLocation();
		directionPlayer.Normalize();
		if (!isPlayed)
		{
			UGameplayStatics::PlaySound2D(this, dragonRush);
			isPlayed = true;
		}
	}
	// 정한 방향으로 빠르게 이동한다
	else if (currentTime >= 4.5 && currentTime < 8)
	{
		SetActorLocation(GetActorLocation() + directionPlayer * DeltaTime * 1200);
	}
	// 저장했던 메인위치로 돌아온다
	else if (currentTime >=8 && currentTime < 10)
	{
		FVector toMain = mainLoc - currentLoc;
		toMain.Normalize();
 		if (currentLoc.Y >= mainLoc.Y + 20 || currentLoc.Y <= mainLoc.Y - 20 || currentLoc.Z >= mainLoc.Z + 20 || currentLoc.Z <= mainLoc.Z - 20)
 		{
 		   SetActorLocation(GetActorLocation() + toMain * DeltaTime * 600);
 		}
	}
	else if (currentTime > 10)
	{
		isShoot1 = false;
		isShoot2 = false;
		isShoot3 = false;
		isPlayed = false;
		currentTime = 2.5;
	}

	}


void AMidBoss::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APooledObject* playerBullet = Cast<APooledObject>(OtherActor);

	if (playerBullet)
	{
		playerBullet->Reset();

		if (myHP > 0)
		{
			myHP -= playerBullet->GetAttackPower();
			hpWidget->SetVisibility(ESlateVisibility::Visible);
			hpWidget->PrintCurrentHealth(myHP, maxHP);
		}
		else
		{
			DestroyMidBoss();
			//DestroyEnemy();
			ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
			if (tengaiGM)
			{
				tengaiGM->AddScore(point);
			}

			GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), FRotator(0, 90, 0));
			GetWorld()->SpawnActor<AItem>(itemFactoryUlti, GetActorLocation() + FVector(0, 0, -100), FRotator(0, 90, 0));
			tengaiGM->playSpeed = 150;

			warningUI = CreateWidget<UWarningWidget>(GetWorld(), warningWidget);
			if (warningUI != nullptr)
			{
				warningUI->AddToViewport();
			}

			Destroy();
		}
	}

	AFence_Vertical* fenceVer = Cast<AFence_Vertical>(OtherActor);
	if (fenceVer != nullptr)
	{
		direction.Y *= -1;
		directionPlayer.Y *= -1;
		return;
	}

	AFence_Horizontal* fenceHor = Cast<AFence_Horizontal>(OtherActor);
	if (fenceHor != nullptr)
	{
		direction.Z *= -1;
		directionPlayer.Z *= -1;
	}
}

void AMidBoss::DestroyMidBoss()
{
	isDead = true;

	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	flipComp->SetHiddenInGame(false);

	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	if (tengaiGM)
	{
		tengaiGM->AddScore(point);
	}

	GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -90), FRotator(0, 90, 0));
	GetWorld()->SpawnActor<AItem>(itemFactoryUlti, GetActorLocation() + FVector(0, 0, -100), FRotator(0, 90, 0));

	direction = FVector(0);

	itemWidget->SetVisibility(ESlateVisibility::Visible);
	itemWidget->PrintMonsterScore(point);

	GetWorldTimerManager().SetTimer(timer, this, &AMidBoss::DestroySelf, 0.8f, false);
}

void AMidBoss::DestroySelf()
{
	Destroy();
}

