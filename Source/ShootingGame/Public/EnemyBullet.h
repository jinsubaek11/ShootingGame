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
	class USphereComponent* sphereComp;
	UPROPERTY(EditDefaultsOnly, Category=EnemyBulletSettings)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, Category=EnemyBulletSettings)
	int8 enemyBulletTrace = 1;

private:
	FVector direction;
<<<<<<< HEAD
<<<<<<< HEAD
	float bulletSpeed = 250;
=======
	float bulletSpeed = 300;
>>>>>>> b95a99b32cacab323193a55547cdd984304772fe
=======
	float bulletSpeed = 300;
>>>>>>> b95a99b32cacab323193a55547cdd984304772fe
	class APlayerFlight* target;
};
