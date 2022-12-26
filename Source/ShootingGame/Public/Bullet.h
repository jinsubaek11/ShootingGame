#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledBulletDespawn, ABullet*, bullet);

UCLASS()
class SHOOTINGGAME_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void SetActive(bool isActiveBullet);
	void SetDeactive();
	void SetLifeSpan(float lifeTime) override;
	virtual void SetDirection(FVector direction);
	void SetIndex(uint16 bulletIndex);
	bool IsActive() const;
	uint16 GetIndex() const;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UArrowComponent* arrowComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

	FOnPooledBulletDespawn OnPooledBulletDespawn;
	bool isActive;

protected:
	float bulletSpeed = 1200.f;
	FVector bulletDirection;

private:
	FTimerHandle lifeSpanTimer;
	uint16 index;
	float lifeSpan;

};
