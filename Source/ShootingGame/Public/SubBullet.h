#pragma once

#include "CoreMinimal.h"
#include "Bullet.h"
#include "SubBullet.generated.h"


UCLASS()
class SHOOTINGGAME_API ASubBullet : public ABullet
{
	GENERATED_BODY()
	
public:
	ASubBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	void SetDirection(FVector start, FVector end);

private:
	FVector GetCalculateLocation(float time) const;

private:
	FVector startPosition;
	FVector endPosition;
	FVector firstControlPosition;
	FVector secondControlPosition;

	float durationTime = 1.4f;
	float accumulateTime;

	uint8 lifeCount = 2;
};
