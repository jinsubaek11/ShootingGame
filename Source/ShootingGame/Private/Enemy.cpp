// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Item.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "Runtime/Engine/public/TimerManager.h"
#include "Bullet.h"
#include "TengaiGameMode.h"
#include "Fence_Vertical.h"
#include "Fence_Horizontal.h"
#include "EnemyBullet.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜리전 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));

	// 메쉬 생성
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
		// Y축따라 직선으로 들어왔다가 다시 오른쪽으로 나가도록
		FVector newLocation = GetActorLocation();
		float deltaY = (FMath::Sin(runningTime + DeltaTime) - FMath::Sin(runningTime));
		newLocation.Y += deltaY * -800.0f;
		runningTime += DeltaTime;
		SetActorLocation(newLocation);
	} 
	if (movingMode == 2)
	{
		// Y축따라 직선으로 들어와서 정지(++시간으로)
		//	FMath::Sin(DeltaTime * PI *2);
		FVector newLocation = GetActorLocation();
		float deltaY = (FMath::Sin(runningTime + DeltaTime) - FMath::Sin(runningTime));
		if (deltaY > 0)
		{
			newLocation.Y += deltaY * -600.0f;
			runningTime += DeltaTime;
			SetActorLocation(newLocation);
			return;
		}
		// 플레이어 방향으로 가는 총알을 스폰
		if (!isShoot)
		{
			GetWorld()->SpawnActor<AEnemyBullet>(EnemyBulFactory, GetActorLocation(), GetActorRotation());
			isShoot = true;
			return;
		}
		// Z축 따라 위로 올라감 (펜스에 부딫혀 사라짐)
		newLocation.Z = newLocation.Z + DeltaTime * enemySpeed;
		SetActorLocation(newLocation);
	}
	else
	{
		// 적 등속이동 p=p0+vt
		direction = GetActorForwardVector();
		SetActorLocation(GetActorLocation() + direction * enemySpeed * DeltaTime);
	}

	
}

void AEnemy::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ABullet* bullet = Cast<ABullet>(OtherActor);

	if (bullet != nullptr)
	{
		if (drawRate <= dropRate)
		{
			GetWorld()->SpawnActor<AItem>(itemFactory, GetActorLocation() + FVector(0, 0, -100), GetActorRotation());
		}

		bullet->Destroy();

		if (myHP > 0)
		{
			myHP -= 1;
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
}
