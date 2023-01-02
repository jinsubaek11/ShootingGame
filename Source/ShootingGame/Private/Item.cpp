// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"
#include "PlayerFlight.h"
#include "Fence_Vertical.h"
#include "Fence_Horizontal.h"

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜리전 생성
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(25));
	boxComp->SetCollisionProfileName(TEXT("ItemPreset"));

	// 메쉬 생성
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlap);

	randomDir.Y = FMath::RandRange(-100.0f, 100.0f);
	randomDir.Z = FMath::RandRange(-100.0f, 100.0f);
	randomDir.Normalize();

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 아이템 스폰시 랜덤한 방향으로 이동하게
	SetActorLocation(GetActorLocation() + randomDir * itemSpeed * DeltaTime, true);

}

void AItem::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerFlight* player = Cast<APlayerFlight>(OtherActor);
	if (player != nullptr)
	{
		player->SetAttackLevel((AttackLevel)(player->GetAttackLevel() + 1));
		Destroy();
		return;
	}

	AFence_Vertical* fenceVer = Cast<AFence_Vertical>(OtherActor);
	if (fenceVer != nullptr)
	{
		randomDir.Y *= -1;
		//UE_LOG(LogTemp, Warning, TEXT("toched v"));
		return;
	}

	AFence_Horizontal* fenceHor = Cast<AFence_Horizontal>(OtherActor);
	if (fenceHor != nullptr)
	{
		randomDir.Z *= -1;
		//UE_LOG(LogTemp, Warning, TEXT("toched h"));
	}
}
