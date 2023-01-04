// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MidBoss.generated.h"

UCLASS()
class SHOOTINGGAME_API AMidBoss : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMidBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MidBossSettings)
	UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MidBossSettings)
	UPaperFlipbookComponent* flipComp;
	UPROPERTY(EditInstanceOnly, Category = MidBossSettings)
	TSubclassOf <class AItem> itemFactory;
	UPROPERTY(EditInstanceOnly, Category = MidBossSettings)
	TSubclassOf <class AItem> itemFactoryUlti;
	UPROPERTY(EditInstanceOnly, category = EnemySettings)
	TSubclassOf<class AEnemyBullet>EnemyBulFactory;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= MidBossSettings)
	uint8 myHP = 50;

	float currentTime = 0;
	FVector direction;
	class APlayerFlight* target;
	bool isShoot1 = false;
	bool isShoot2 = false;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	void DestroyEnemy();
// 	void SetDirection();
};
