// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTINGGAME_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	// ï¿½Ú½ï¿½ ï¿½Ý¸ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UBoxComponent* boxComp;
	// ï¿½Þ½ï¿½ ï¿½ï¿½ï¿½ï¿½
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UStaticMeshComponent* meshComp;

	// ï¿½ï¿½ ï¿½Ìµï¿½ ï¿½Óµï¿½ ï¿½ï¿½ï¿½ï¿½
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	float enemySpeed=400;
	// ï¿½ï¿½ ï¿½Ìµï¿½ ï¿½ï¿½ï¿½ï¿½
	FVector direction;
	// ï¿½Ìµï¿½ ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	int32 movingMode = 0;
<<<<<<< HEAD
=======

	float runningTime = 0;
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442

	float runningTime = 0;

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Æ® ï¿½ï¿½ï¿½ï¿½Ã¢ ï¿½ï¿½ï¿½ï¿½ï¿½
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	TSubclassOf <class AItem> itemFactory;

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ï¿½ ï¿½Ô¼ï¿½
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ ï¿½ï¿½ï¿½ È®ï¿½ï¿½
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float dropRate=0.05;
	float drawRate;

<<<<<<< HEAD
	// ï¿½ï¿½ Ã¼ï¿½ï¿½
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	int8 myHP=1;

=======
	// Àû Ã¼·Â
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	int8 myHP=1;

	// ÀûÃÑ¾Ë ºí·çÇÁ¸°Æ® ¼³Á¤
>>>>>>> bfc00b683e7e97e60e0897d8de0ca84f82dd0442
	UPROPERTY(EditInstanceOnly, category = EnemySettings)
	TSubclassOf<class AEnemyBullet>EnemyBulFactory;

	bool isShoot = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
