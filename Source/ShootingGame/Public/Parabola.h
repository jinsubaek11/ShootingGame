// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Parabola.generated.h"

UCLASS()
class SHOOTINGGAME_API AParabola : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParabola();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	FVector position;
	FVector velocity;
	FVector gravity;
	float acceleration;
	float t;
	float theta = FMath::DegreesToRadians(60);

};
