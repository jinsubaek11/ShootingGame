// Fill out your copyright notice in the Description page of Project Settings.


#include "Parabola.h"

// Sets default values
AParabola::AParabola()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AParabola::BeginPlay()
{
	Super::BeginPlay();
	
	position = GetActorLocation();
	velocity = FVector(0, 100, 100);
	gravity = FVector(0, 0, -9.8);
	acceleration = 2.f;
}

// Called every frame
void AParabola::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//t += DeltaTime;

	//FVector pos = position + velocity * t + 0.5 * acceleration * t * t;

	velocity += gravity * DeltaTime * 10;
	position += velocity * DeltaTime * 10;

	SetActorLocation(position);

	//position = velocity * FMath::Sin(theta) * t - 0.5 * gravity * t * t;

	UE_LOG(LogTemp, Warning, TEXT("%f, %f, %f"), position.X, position.Y, position.Z);
	//SetActorLocation(position);
}

