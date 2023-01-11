// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EnemyMovingType: uint8
{
	GO_STRAIGHT,
	RIGHT_RETURN_BACK,
	RIGHT_SHOOT_UP,
	RIGHT_CIRCLE_LEFT,
};

UCLASS()
class SHOOTINGGAME_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere, category = EnemySettings)
	class UPaperFlipbookComponent* flipbookComp;

 	UPROPERTY(EditAnywhere, Category = EnemySettings)
 	EnemyMovingType movingType;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	float enemySpeed=400;

	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	TSubclassOf <class AItem> itemFactory;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	TSubclassOf <class AItem> itemFactoryUlti;
	UPROPERTY(EditInstanceOnly, category = EnemySettings)
	TSubclassOf<class AEnemyBullet>EnemyBulFactory;

	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float dropRate=0.05;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float dropRateUlti = 0.05;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float myHP;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemSettings)
	class UWidgetComponent* widgetComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemSettings)
	class UWidgetComponent* hpWidgetComp;
	class UItemWidget* itemWidget;
	class UHPWidget* hpWidget;

	UPROPERTY(EditAnywhere, Category=SoundEffect)
	USoundBase* Enemydown;

	FTimerHandle timer;
	FTimerHandle TimerHandle;
	FTimerHandle TimerHandle2;
	FTimerHandle materialTimer;

	FVector direction;
	float drawRate;
	float drawRateUlti;
	float runningTime = 0;
	float deltaTimeForFunc;
	bool isShoot = false;
	bool isDead = false;
	bool canMove = false;
	int32 point = 30;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void DestroyEnemy();
	void DestroySelf();
	void GoStraight();
	void RightReturnBack();
	void RightShootUp();
	void RightCircleLeft();

};

