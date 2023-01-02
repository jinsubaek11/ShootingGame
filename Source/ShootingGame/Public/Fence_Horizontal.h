// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fence_Horizontal.generated.h"

UCLASS()
class SHOOTINGGAME_API AFence_Horizontal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFence_Horizontal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
