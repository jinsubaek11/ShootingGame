#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerFlight.generated.h"

UENUM()
enum class AttackLevel : uint8
{
	WEAK = 1,
	NORMAL,
	STRONG
};

UCLASS()
class SHOOTINGGAME_API APlayerFlight : public APawn
{
	GENERATED_BODY()

public:
	APlayerFlight();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	int32 GetLifeCount() const;
	int32 GetMaxLifeCount() const;
	int32 GetUltimateCount() const;
	int32 GetMaxUltimateCount() const;
	bool GetIsDead() const;
	bool GetIsInvincibility() const;
	void SetAttackLevel(AttackLevel level);
	void LifeCalculator();
	void AddUltimateCount();
	uint8 GetAttackLevel() const; 
	class ANormalBulletPool* GetNormalBulletPool();

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnFenceHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	void SetAttackBarrier(AttackLevel level);
	void HorizontalInput(float value);
	void VerticalInput(float value);
	void Fire(float value);
	void ShootStrongAttack();
	void ShootUltimate();
	void Reset();
	void SetFalseInvincibility();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditAnywhere, Category = SoundEffect)
	USoundBase* playerDown;
	UPROPERTY(EditAnywhere, Category = SoundEffect)
	USoundBase* playerFire;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float moveSpeed = 800.f;
	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float shootCoolTime = 0.1f;
	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	AttackLevel attackLevel = AttackLevel::WEAK;
	UPROPERTY()
	class ASubBulletPool* subBulletPool;
private:
	UPROPERTY()
	TArray<class AAttackBarrier*> attackBarriers;
	UPROPERTY()
	TArray<class AEnemy*> enemies;
	UPROPERTY()
	class ANormalBulletPool* normalBulletPool;
	UPROPERTY()
	class AStrongBulletPool* strongBulletPool;
	UPROPERTY()
	class AUltimateBullet* ultimate;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AItem> powerItem;

	class ATengaiGameMode* tengaiGM;
	FTimerHandle timer;

	FVector direction;
	FVector position;
	FVector velocity;
	FVector gravity;

	int32 lifeCount = 4;
	int32 const MAX_LIFE_COUNT = 4;
	int32 ultimateCount = 2;
	int32 const MAX_ULTIMATE_COUNT = 3;
	float ultimateDurationTime;
	float ultimateMaxDurationTime = 3.f;
	bool isFireUltimate = false;
	bool isShooting = false;
	bool readyToSubAttack = false;
	bool isFireSubAttack = false;
	float shootWaitingTime = 0.f;
	float subAttackWaitingTime = 0.f;
	float subAttackCoolTime = 1.f;
	bool isDead = false;
	bool isInvincibility = false;
	float deadWaitingTime = 0.f;
	float deadCoolTime = 2.f;

	const int8 MIN_DEGREE = -2;
	const int8 MAX_DEGREE =  2;
	const int8 COUNT_CONTROL_VAR = 8;
};

