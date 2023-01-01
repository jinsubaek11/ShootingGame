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
	void SetMovingPath(uint16 pathCount);
	void SetMovingTimeLine();
	void Shoot(AttackType attackType);
	void FanShoot();
	void SequenceShoot();
	void SequenceSpiralShoot();
	void SpiralExplosion();
	void RadialExplosion();
	AttackType SelectAttackType();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

private:
	class UEnemyMovement* movementComp;
	class AEnemyBulletPool* enemyBulletPool;

	float time;
	bool isFired;

	TArray<FMovement> customPath;
	TArray<float> timeLine;

	FTimerHandle timer;
	int16 fanShootCallsRemaining = 10;
	int16 sequenceShootCallsRemaining = 60;
	int16 sequenceSpiralShootCallsRemaining = 180;
	int16 spiralExplosionCallsRemaining = 15;
	int16 radialExplosionCallsRemaining = 15;
};
