#pragma once

#include "CoreMinimal.h"
#include "PooledObject.h"
#include "PooledBullet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPooledNormalBulletDespawn, APooledBullet*, pooledBullet);

UCLASS()
class SHOOTINGGAME_API APooledBullet : public APooledObject
{
	GENERATED_BODY()

public:
	APooledBullet();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void Reset() override;
	virtual void SetActive(bool value) override;
	virtual void SetDeactive() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UArrowComponent* arrowComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

	FOnPooledNormalBulletDespawn OnPooledNormalBulletDespawn;
};
