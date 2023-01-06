// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBullet.generated.h"

UCLASS()
class SHOOTINGGAME_API AEnemyBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category=EnemyBulletSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, Category=EnemyBulletSettings)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category=EnemyBulletSettings)
	int32 enemyBulletTrace = 1;
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	TSubclassOf<class AItem> powerItem;
	class ATengaiGameMode* tengaiGM;


private:
	FVector direction;
	float bulletSpeed = 600;
	class APlayerFlight* target;

};
