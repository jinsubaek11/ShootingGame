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
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=MidBossSettings)
	class UPaperFlipbookComponent* flipComp;
	UPROPERTY(EditDefaultsOnly, Category = MidBossSettings)
	TSubclassOf <class AItem> itemFactory;
	UPROPERTY(EditDefaultsOnly, Category = MidBossSettings)
	TSubclassOf <class AItem> itemFactoryUlti;
	UPROPERTY(EditDefaultsOnly, Category = EnemySettings)
	TSubclassOf<class AEnemyBullet>EnemyBulFactory;
	class UWarningWidget* warningUI;
	UPROPERTY(EditAnywhere, Category = MyDefaultSettings)
	TSubclassOf <class UWarningWidget> warningWidget;
	UPROPERTY(EditDefaultsOnly, Category = SoundEffect)
	USoundBase* dragonSpawned;
	UPROPERTY(EditDefaultsOnly, Category = SoundEffect)
	USoundBase* dragonRush;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category= MidBossSettings)
	int32 myHP = 10;

	float currentTime = 0;
	FVector direction;
	FVector directionPlayer;
	bool isShoot1 = false;
	bool isShoot2 = false;
	bool isShoot3 = false;
	bool isPlayed = false;
	FVector currentLoc;
	FVector mainLoc;
	int32 point = 100;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
// 	void DestroyEnemy();
// 	void SetDirection();
	void MidBossStart();
};
