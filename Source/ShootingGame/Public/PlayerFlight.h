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
	void SetAttackLevel(uint8 level);
	uint8 GetAttackLevel() const;
	class ABulletPool* GetBulletPool() const;

private:
	void SetAttackBarrier(uint8 level);
	void HorizontalInput(float value);
	void VerticalInput(float value);
	void Fire(float value);
	void ShootStrongAttack();
	void ShootUltimate();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxcomp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshcomp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerSettings)
	float moveSpeed = 800.f;
	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	float shootCoolTime = 0.1f;
	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	uint8 attackLevel = 1;

private:
	UPROPERTY()
	TArray<class AAttackBarrier*> attackBarriers;
	UPROPERTY()
	class ABulletPool* bulletPool;
	class AUltimateBullet* ultimate;

	FVector direction;

	int ultimateCount = 2;
	float ultimateDurationTime;
	float ultimateMaxDurationTime = 3.f;
	bool isFireUltimate = false;
	bool isShooting = false;
	bool readyToSubAttack = false;
	bool isFireSubAttack = false;
	float shootWaitingTime = 0.f;
	float subAttackWaitingTime = 0.f;
	float subAttackCoolTime = 1.f;

	const int8 MIN_DEGREE = -2;
	const int8 MAX_DEGREE =  2;
	const int8 COUNT_CONTROL_VAR = 8;
};

