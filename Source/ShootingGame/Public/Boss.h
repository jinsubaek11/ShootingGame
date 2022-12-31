#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EnemyMovement.h"

#include "Boss.generated.h"

UCLASS()
class SHOOTINGGAME_API ABoss : public AActor
{
	GENERATED_BODY()
	
public:	
	ABoss();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	void SetMovingPath(uint16 pathCount);
	void SetMovingTimeLine();
	void Shoot();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

private:
	class UEnemyMovement* movementComp;

	float time;
	bool isFired;

	TArray<FMovement> customPath;
	TArray<float> timeLine;
};
