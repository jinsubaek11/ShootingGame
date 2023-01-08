// Fill out your copyright notice in the Description page of Project Settings.


#include "Stoper.h"
#include "components/BoxComponent.h"
#include "TengaiGameMode.h"
#include "PreBoss.h"

// Sets default values
AStoper::AStoper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	boxComp->SetBoxExtent(FVector(50));
}

// Called when the game starts or when spawned
void AStoper::BeginPlay()
{
	Super::BeginPlay();
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AStoper::OnOverlap);
}

// Called every frame
void AStoper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStoper::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATengaiGameMode* gm = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	if (gm != nullptr)
	{
		gm->playSpeed = 0;


		if (!gm->encountMidBoss)
		{
			// 중간보스

			gm->encountMidBoss = true;
		}
		else
		{
			// 보스
			UE_LOG(LogTemp, Warning, TEXT("pre boss appear"));
			GetWorld()->SpawnActor<APreBoss>(preBoss, GetActorLocation() + FVector(0, -250, -200), FRotator::ZeroRotator);
		}
	}
}
