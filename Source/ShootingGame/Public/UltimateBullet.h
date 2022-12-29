#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UltimateBullet.generated.h"

UCLASS()
class SHOOTINGGAME_API AUltimateBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	AUltimateBullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public: 
	void SetActive(bool isActive);

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult
	);

public:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* niagaraComp;

private:
	bool isUltimateActive = false;
};
