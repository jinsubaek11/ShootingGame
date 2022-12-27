#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackBarrier.generated.h"

UCLASS()
class SHOOTINGGAME_API AAttackBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	AAttackBarrier();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	void Shoot(FVector target);
	void SetStartAngle(float degree);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UArrowComponent* arrowComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class ABullet* bulletfactory;
	UPROPERTY()
	class APlayerFlight* player;

	TSoftClassPtr<AActor> subBulletBP = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'")));

private:
	float angle;
	float delay = 1.f;
	float accTime = 0.f;
	float accRotation = 0.f;
	float distance = 150.f;
};
