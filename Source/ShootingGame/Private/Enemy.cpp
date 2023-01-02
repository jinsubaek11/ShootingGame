// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Item.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "Runtime/Engine/public/TimerManager.h"
#include "TengaiGameMode.h"
#include "Fence_Vertical.h"
#include "Fence_Horizontal.h"
#include "EnemyBullet.h"
#include "PooledObject.h"
#include "PooledSubBullet.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// ï¿½Ú½ï¿½ ï¿½Ý¸ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionProfileName(TEXT("EnemyPreset"));

	// ï¿½Þ½ï¿½ ï¿½ï¿½ï¿½ï¿½
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnOverlap);
	drawRate = FMath::RandRange(0.0f, 1.0f);

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (movingMode == 1)
	{		
<<<<<<< HEAD
=======
		// YÃàµû¶ó Á÷¼±À¸·Î µé¾î¿Ô´Ù°¡ ´Ù½Ã ¿À¸¥ÂÊÀ¸·Î ³ª°¡µµ·Ï
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
		FVector newLocation = GetActorLocation();
		float deltaY = (FMath::Sin(runningTime + DeltaTime) - FMath::Sin(runningTime));
		newLocation.Y += deltaY * -700.0f;
		runningTime += DeltaTime;
		SetActorLocation(newLocation);
	} 
	else if (movingMode == 2)
	{
<<<<<<< HEAD
=======
		// YÃàµû¶ó Á÷¼±À¸·Î ºÎµå·´°Ô µé¾î¿Í¼­ Á¤Áö
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
		FVector newLocation = GetActorLocation();
		runningTime += DeltaTime;
		float deltaY = FMath::Sin(runningTime);
		if (deltaY > 0)
		{
			newLocation.Y += deltaY * -3.0f;
			SetActorLocation(newLocation);
			return;
		}
<<<<<<< HEAD
=======
		// ÇÃ·¹ÀÌ¾î ¹æÇâÀ¸·Î °¡´Â ÃÑ¾ËÀ» ½ºÆù
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
		else if (!isShoot)
		{
			GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation(), GetActorRotation());
			isShoot = true;
			return;
		}
<<<<<<< HEAD

=======
		// ZÃà µû¶ó À§·Î ¿Ã¶ó°¨ (Ææ½º¿¡ ºÎ‹HÇô »ç¶óÁü)
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
		newLocation.Z = newLocation.Z + DeltaTime * enemySpeed;
		SetActorLocation(newLocation);
	}
	else if (movingMode == 3)
	{
<<<<<<< HEAD

=======
	// ³ª¿ÀÀÚ¸¶ÀÚ ¿øÀ» ±×¸®¸ç È¸Àü ÈÄ ¿ÞÂÊÀ¸·Î ÅðÀå
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
		FVector newLocation = GetActorLocation();
		runningTime += DeltaTime;
		float deltaY = (FMath::Sin((runningTime + DeltaTime) * 2) - FMath::Sin(runningTime * 2));
		float DeltaZ = FMath::Sin(runningTime * 2);
		newLocation.Y -= 3.0f;
		newLocation.Y -= deltaY * 400.0f;
		newLocation.Z += DeltaZ * 5.0f;
<<<<<<< HEAD

=======
		SetActorLocation(newLocation);
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
	}
	else
	{
		// ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½Ìµï¿½ p=p0+vt
		direction = GetActorForwardVector();
		SetActorLocation(GetActorLocation() + direction * enemySpeed * DeltaTime);
	}	
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APooledObject* playerBullet = Cast<APooledObject>(OtherActor);

	if (playerBullet)
	{
		if (drawRate <= dropRate)
		{
			GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
		}
	
		//UE_LOG(LogTemp, Warning, TEXT("%f"), playerBullet->GetAttackPower());
		playerBullet->Reset();
<<<<<<< HEAD

=======
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
		if (myHP > 0)
		{
			myHP -= playerBullet->GetAttackPower();
		} 
		else
		{
			Destroy();
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
