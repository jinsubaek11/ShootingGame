#pragma once

#include "CoreMinimal.h"
#include "PooledObject.h"
#include "PooledStrongBullet.generated.h"

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

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UArrowComponent* arrowComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

	FOnPooledStrongBulletDespawn OnPooledStrongBulletDespawn;
};
