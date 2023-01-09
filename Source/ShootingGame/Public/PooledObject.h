#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObject.generated.h"

UCLASS()
class SHOOTINGGAME_API APooledObject : public AActor
{
	GENERATED_BODY()
	
public:	
	APooledObject();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Reset();
	virtual void SetActive(bool value);
	virtual void SetDeactive();
	virtual void SetDirection(FVector start, FVector end);
	virtual float GetAttackPower();
	void SetLifeSpan(float lifeTime) override;
	void SetSpeedScale(float scale);
	void SetIndex(uint16 objectIndex);
	bool IsActive() const;
	uint16 GetIndex() const;

public:
	bool isActive;

protected:
	FTimerHandle lifeSpanTimer;
	FVector direction;
	float speed;
	uint16 index;
	float lifeSpan;

};
