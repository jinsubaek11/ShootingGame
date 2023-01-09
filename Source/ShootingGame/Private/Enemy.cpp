// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Item.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "Runtime/Engine/public/TimerManager.h"
#include "PooledObject.h"
#include "PooledSubBullet.h"
#include "Fence_Horizontal.h"
#include "Fence_Vertical.h"
#include "EnemyBullet.h"
#include "PaperFlipbookComponent.h"
#include "TengaiGameMode.h"
#include "ItemWidget.h"
#include "HPWidget.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionProfileName(TEXT("EnemyPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	flipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	flipbookComp->SetRelativeScale3D(FVector(3));
	flipbookComp->SetupAttachment(RootComponent);
	flipbookComp->SetHiddenInGame(true);
	flipbookComp->Stop();

	widgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	widgetComp->SetupAttachment(RootComponent);
	widgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	widgetComp->SetPivot(FVector2D(-0.15, 0.2));

	hpWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HP Widget Component"));
	hpWidgetComp->SetupAttachment(RootComponent);
	hpWidgetComp->SetRelativeRotation(FRotator(0, -90, 0));
	hpWidgetComp->SetPivot(FVector2D(0.5, 5.0));
	hpWidgetComp->SetDrawSize(FVector2D(120, 13));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);

	drawRate = FMath::RandRange(0.0f, 1.0f);
	drawRateUlti = FMath::RandRange(0.0f, 1.0f);
	
	itemWidget = Cast<UItemWidget>(widgetComp->GetWidget());
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
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDead)
	{
		SetActorLocation(GetActorLocation() + direction * enemySpeed * DeltaTime);
		return;
	}
	// 오른쪽에서 들어왔다가 다시 오른쪽으로 되돌아감
	if (movingMode==1)
	{
		FVector newLocation = GetActorLocation();
		float deltaY = (FMath::Sin(runningTime + DeltaTime) - FMath::Sin(runningTime));
		newLocation.Y += deltaY * -900.0f + 3.5f;
		runningTime += DeltaTime;
		SetActorLocation(newLocation);

	} 
	// 오른쪽에서 들어왔다가 총알 쏘고 위로 올라감
	else if (movingMode == 2)
	{
		FVector newLocation = GetActorLocation();
		runningTime += DeltaTime;
		float deltaY = FMath::Sin(runningTime * 2);
		if (deltaY > 0)
		{
			newLocation.Y += deltaY * -6.0f;
			SetActorLocation(newLocation);
			return;
		}
		else if (!isShoot)
		{
			GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation(), GetActorRotation());
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
				{
						GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation(), GetActorRotation());
						//UE_LOG(LogTemp, Warning, TEXT("Shoot Timer"));
				}, 0.1f, false);
			isShoot = true;
			//UE_LOG(LogTemp, Warning, TEXT("Shoot"));
			return;
		}
 		newLocation.Z = newLocation.Z + DeltaTime * enemySpeed * 2;
 		SetActorLocation(newLocation);
	}
	// 오른쪽에서 들어와 위로 원을 그리며 한바퀴 회전 후 왼쪽으로 퇴장
	else if (movingMode == 3)
	{

		FVector newLocation = GetActorLocation();
		runningTime += DeltaTime;
		float deltaY = (FMath::Sin((runningTime + DeltaTime) * 1.0f) - FMath::Sin(runningTime * 1.0f));
		float deltaZ = FMath::Sin(runningTime * 1.0f);
		newLocation.Y -= 3.0f;
		newLocation.Y -= deltaY * 400.0f;
		newLocation.Z += deltaZ * 5.0f;

		SetActorLocation(newLocation);
	}
	// 그 외 오른쪽에서 왼쪽으로 직진
	else
	{

		direction = GetActorForwardVector();
		SetActorLocation(GetActorLocation() + direction * enemySpeed * DeltaTime * 2.0f);
	}	
}

void AEnemy::DestroyEnemy()
{
	isDead = true;

	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetHiddenInGame(true);
	flipbookComp->SetHiddenInGame(false);
	flipbookComp->Play();
	UGameplayStatics::PlaySound2D(this, Enemydown, 1.0f, 1.0f, 0.2f);

	GetWorldTimerManager().SetTimer(timer, this, &AEnemy::DestroySelf, 0.8f, false);

	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	if (tengaiGM)
	{
		tengaiGM->AddScore(point);
	}

	direction = FVector(0);

	itemWidget->SetVisibility(ESlateVisibility::Visible);
	itemWidget->PrintMonsterScore(point);	
}

void AEnemy::DestroySelf()
{
	Destroy();
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APooledObject* playerBullet = Cast<APooledObject>(OtherActor);

	if (playerBullet)
	{
		playerBullet->Reset();
		
		if (myHP > 0)
		{
			myHP -= playerBullet->GetAttackPower();
			hpWidget->SetVisibility(ESlateVisibility::Visible);
			hpWidget->PrintCurrentHealth(myHP, MaxHP);
		}
		else
		{
			SetActorEnableCollision(false);

			if (drawRate <= dropRate)
			{
				GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), FRotator(0, 90, 0));
				UE_LOG(LogTemp, Warning, TEXT("power item spawned"));
			}

			if (drawRateUlti <= dropRateUlti)
			{
				GetWorld()->SpawnActor<AItem>(itemFactoryUlti, GetActorLocation() + FVector(0, 0, -100), FRotator(0, 90, 0));
				UE_LOG(LogTemp, Warning, TEXT("ult item spawned"));
			}
			DestroyEnemy();
		}
	}

	AFence_Horizontal* fenceH = Cast<AFence_Horizontal>(OtherActor);

	if (fenceH != nullptr)
	{
		Destroy();
	}

	AFence_Vertical* fenceV = Cast<AFence_Vertical>(OtherActor);

	if (fenceV != nullptr)
	{
		Destroy();
	}

}