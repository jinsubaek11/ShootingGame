#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EnemyMovement.h"

#include "Boss.generated.h"

UENUM()
enum class AttackType: uint8
{
	FAN_SHOOT,
	SEQUENCE_SHOOT,
	SEQUENCE_SPIRAL_SHOOT,
	SPIRAL_EXPLOSION,
	RADIAL_EXPLOSION,
};

UENUM()
enum class AnimationType : uint8
{
	WALK,
	WALK_WITH_SWORD,
	ATTACK,
	DEAD,
};

UCLASS()
class SHOOTINGGAME_API ABoss : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoss();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void SetAnimation(AnimationType animationType);
	void SetAnimationComponent(AnimationType animationType);

	void SetMovingPath(uint16 pathCount);
	void SetMovingTimeLine();

	void Shoot(AttackType attackType);
	void FanShoot();
	void SequenceShoot();
	void SequenceSpiralShoot();
	void SpiralExplosion();
	void RadialExplosion();
	AttackType SelectAttackType();

	void DestroySelf();
	void RecoverHPBar();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);


public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UPaperFlipbookComponent* attackFlipBookComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UPaperFlipbookComponent* normalWalkFlipBookComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UPaperFlipbookComponent* walkWithSwordFlipBookComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UPaperFlipbookComponent* deadFlipBookComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemSettings)
	class UWidgetComponent* widgetComp;

private:
	class UEnemyMovement* movementComp;
	class AEnemyBulletPool* enemyBulletPool;

	AnimationType currentAnimationType = AnimationType::WALK_WITH_SWORD;
	class UPaperFlipbookComponent* currentFlipBookComponent;
	class UItemWidget* itemWidget;
	class UHPWidget* bossHPWidget;

	float time;
	bool isFired;
	bool isFiredComplete;

	FVector startOrigin;
	FVector attackStartOrigin;
	TArray<FMovement> customPath;
	TArray<float> timeLine;

	FTimerHandle timer;
	int32 fanShootCallsRemaining = 10;
	int32 sequenceShootCallsRemaining = 40;
	int32 sequenceShootCallsMax = 40;
	int32 sequenceSpiralShootCallsRemaining = 180;
	int32 spiralExplosionCallsRemaining = 15;
	int32 radialExplosionCallsRemaining = 15;

	bool isPlayedStartAnimation;
	float startAnimationDuration = 3.f;
	float startAnimationPlayTime;

	bool isDead;
	int32 hp = 10;
	int32 maxHP = 10;
	int32 point = 200;

	FTimerHandle hpTimer;
	int32 hpRecoverRemaining = 100;
	int32 hpRecoverRemainingMax = 100;
};
