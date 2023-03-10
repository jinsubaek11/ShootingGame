#pragma once

#include "CoreMinimal.h"
#include "PooledObject.h"
#include "PooledStrongBullet.generated.h"

UENUM()
enum class StrongBulletAnimationType
{
	STOP,
	RUN,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledStrongBulletDespawn, APooledStrongBullet*, pooledStrongBullet);

UCLASS()
class SHOOTINGGAME_API APooledStrongBullet : public APooledObject
{
	GENERATED_BODY()
	
public:
	APooledStrongBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Reset() override;
	virtual void SetActive(bool value) override;
	virtual void SetDeactive() override;
	virtual float GetAttackPower() override;

private:
	void SetAnimation(StrongBulletAnimationType type);
	void ShootStrongSubBullet();
	void SetIsFiredFalse();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UArrowComponent* arrowComp;

	class UPaperFlipbookComponent* runFlipbookComp;
	class UPaperFlipbookComponent* stopFlipbookComp;
	class UPaperFlipbookComponent* rotateFlipbookComp;

	FOnPooledStrongBulletDespawn OnPooledStrongBulletDespawn;

private:
	float att = 2;
	class AEnemy* enemy;
	class AMidBoss* midBoss;
	class APreBoss* preBoss;
	class ABoss* boss;
	FVector targetLocation;
	bool isFired;
	FTimerHandle timer;
	FTimerHandle shootTimer;
	FTimerHandle rotateTimer;
	int32 randomAttackType = 0;
	int32 ShootStrongSubBulletRemaining = 3;
};
