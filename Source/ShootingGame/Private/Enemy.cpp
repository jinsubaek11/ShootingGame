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

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �ڽ� �ݸ��� ����
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionProfileName(TEXT("EnemyPreset"));

	// �޽� ����
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

	flipbookComp = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Flipbook"));
	flipbookComp->SetRelativeScale3D(FVector(3));
	flipbookComp->SetupAttachment(RootComponent);
	flipbookComp->SetHiddenInGame(true);
	flipbookComp->Stop();

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);

	drawRate = FMath::RandRange(0.0f, 1.0f);
	drawRateUlti = FMath::RandRange(0.0f, 1.0f);
	


	//flipbookComp->Deactivate();
	//flipbookComp->Stop();


}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isDead) return;

	if (movingMode==1)
	{
		FVector newLocation = GetActorLocation();
		float deltaY = (FMath::Sin(runningTime + DeltaTime) - FMath::Sin(runningTime));
		newLocation.Y += deltaY * -900.0f + 3.5f;
		runningTime += DeltaTime;
		SetActorLocation(newLocation);

	} 
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
	else if (movingMode == 3)
	{
	// �����ڸ��� ���� �׸��� ȸ�� �� �������� ����
		FVector newLocation = GetActorLocation();
		runningTime += DeltaTime;
		float deltaY = (FMath::Sin((runningTime + DeltaTime) * 1.0f) - FMath::Sin(runningTime * 1.0f));
		float deltaZ = FMath::Sin(runningTime * 1.0f);
		newLocation.Y -= 3.0f;
		newLocation.Y -= deltaY * 400.0f;
		newLocation.Z += deltaZ * 5.0f;

		SetActorLocation(newLocation);
	}
	else
	{
		direction = GetActorForwardVector();
		SetActorLocation(GetActorLocation() + direction * enemySpeed * DeltaTime);
	}	
}

void AEnemy::DestroyEnemy()
{
	isDead = true;

	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	meshComp->SetHiddenInGame(true);
	flipbookComp->SetHiddenInGame(false);
	flipbookComp->Play();

	GetWorldTimerManager().SetTimer(timer, this, &AEnemy::DestroySelf, 0.8f, false);

	//int32 playbackPosition = flipbookComp->GetPlaybackPositionInFrames();
	//int32 filpbookLength = flipbookComp->GetFlipbookLengthInFrames();

	//if (playbackPosition == filpbookLength - 1)
	//{
	//	//flipbookComp->Stop();
	//}
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
// 		if (drawRate <= dropRate)
// 		{
// 			GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
// 		}
// 	
		playerBullet->Reset();
		
		if (myHP > 0)
		{
			myHP -= playerBullet->GetAttackPower();
		}
		else
		{
			if (drawRate <= dropRate)
			{
				GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
				UE_LOG(LogTemp, Warning, TEXT("Item Spawned1"));
			}
			if (drawRateUlti <= dropRateUlti)
			{
				GetWorld()->SpawnActor<AItem>(itemFactoryUlti, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
				UE_LOG(LogTemp, Warning, TEXT("Item Spawned2"));
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


	//APooledSubBullet* subBullet = Cast<APooledSubBullet>(OtherActor);

	//// reset moving time overriding APooledObject
	//if (subBullet)
	//{
	//	subBullet->ResetMovingTime();
	//	subBullet->SetDeactive();
	//}
	//else
	//{
	//	APooledObject* bullet = Cast<APooledObject>(OtherActor);
	//	
	//	if (bullet)
	//	{
	//		bullet->SetDeactive();
	//	}
	//}

	//if (drawRate <= dropRate)
	//{
	//	GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
	//}

	//Destroy();
}