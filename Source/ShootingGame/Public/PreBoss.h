#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PreBoss.generated.h"

UENUM()
enum class PreBossAttackType
{
	LINE_SHOOT,
	RANDOM_SHOOT
};

UCLASS()
class SHOOTINGGAME_API APreBoss : public AActor
{
	GENERATED_BODY()
	
public:	
	APreBoss();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void LineShoot();
	void RandomShoot();
	void DestroySelf();
	void PlayExplosionAnimation();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);

public:
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;
	UPROPERTY(EditAnywhere)
	class UPaperSpriteComponent* spriteComp;
	UPROPERTY(EditAnywhere, category = EnemySettings)
	class UPaperFlipbookComponent* explosionFlipbookComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemSettings)
	class UWidgetComponent* widgetComp;
	class UItemWidget* itemWidget;
	class UHPWidget* bossHPWidget;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABoss> boss;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isDead;


private:
	friend class ABoss;
	class AEnemyBulletPool* enemyBulletPool;

	FVector targetLocation;

	FTimerHandle shootTimer;
	FTimerHandle destroyTimer;
	int32 lineShootCallsRemaining = 5;
	int32 lineShootCallsMax = 5;
	int32 randomShootCallsRemaining = 10;
	int32 randomShootCallsMax = 10;

	float shootCoolTime = 3.f;
	float shootWaitTime;

	float hp = 10.f;
	float maxHP = 10.f;
	float point = 200.f;

	bool isPlayExplosionAnimation;
	int32 playExplosionAnimationRemaining = 5;
	FTimerHandle explosionAnimationTimer;
	TArray<FVector> explosionPosition;
	int32 explosionIndex;
};
