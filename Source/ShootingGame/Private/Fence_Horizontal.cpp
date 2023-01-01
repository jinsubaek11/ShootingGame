// Fill out your copyright notice in the Description page of Project Settings.


#include "Fence_Horizontal.h"
#include "TengaiGameMode.h"

// Sets default values
AFence_Horizontal::AFence_Horizontal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFence_Horizontal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFence_Horizontal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 기본 플레이 속도 적용
	AGameModeBase* gm = GetWorld()->GetAuthGameMode();
	ATengaiGameMode* tengaiGM = Cast<ATengaiGameMode>(gm);
	float spd = tengaiGM->playSpeed;
	FVector newLoca = GetActorLocation();
	newLoca.Y = newLoca.Y + spd * DeltaTime;
	SetActorLocation(newLoca);
}

