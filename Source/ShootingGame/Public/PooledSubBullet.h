#pragma once

#include "CoreMinimal.h"
#include "PooledObject.h"
#include "PooledSubBullet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledSubBulletDespawn, APooledSubBullet*, pooledSubBullet);

UCLASS()
class SHOOTINGGAME_API APooledSubBullet : public APooledObject
{
	GENERATED_BODY()
	
public:
	APooledSubBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void ResetMovingTime();
	virtual void SetDirection(FVector start, FVector end) override;
	virtual void Reset() override;
	virtual void SetActive(bool value) override;
	virtual void SetDeactive() override;
	virtual float GetAttackPower() override;


private:
	FVector GetCalculateLocation(float time) const;

public:	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UArrowComponent* arrowComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;
	
	FOnPooledSubBulletDespawn OnPooledSubBulletDespawn;

private:
	FVector startPosition;
	FVector endPosition;
	FVector firstControlPosition;
	FVector secondControlPosition;

	float durationTime = 1.5f;
	float movingTime;

	uint8 lifeCount = 2;
	float att = 0.1;
};
