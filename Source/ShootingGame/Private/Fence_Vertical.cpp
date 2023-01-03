// Fill out your copyright notice in the Description page of Project Settings.


#include "Fence_Vertical.h"
#include "TengaiGameMode.h"

// Sets default values
AFence_Vertical::AFence_Vertical()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFence_Vertical::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFence_Vertical::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	 //기본 플레이 속도 적용
	auto tengaiGM = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	if (tengaiGM != nullptr)
	{
		float spd = tengaiGM->playSpeed;
		FVector newLoca = GetActorLocation();
		newLoca.Y = newLoca.Y + spd * DeltaTime;
		SetActorLocation(newLoca);
	}

}

