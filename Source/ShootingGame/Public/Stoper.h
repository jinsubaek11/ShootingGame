// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Stoper.generated.h"

UCLASS()
class SHOOTINGGAME_API AStoper : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStoper();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APreBoss> preBoss;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};