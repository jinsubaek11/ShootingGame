// Fill out your copyright notice in the Description page of Project Settings.


#include "Test.h"
#include "components/BoxComponent.h"
#include "components/StaticMeshComponent.h"

// Sets default values
ATest::ATest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50));
	boxComp->SetCollisionProfileName(TEXT("TestPreset"));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	meshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ATest::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ATest::OnOverlap);
}

// Called every frame
void ATest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATest::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap")); \
	GetType();
}

void ATest::GetType()
{
}