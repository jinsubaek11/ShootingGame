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

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex
	);

public:
	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;
	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* niagaraComp;

private:
	bool isOverlappingMidBoss = false;
	bool isOverlappingPreBoss = false;
	bool isOverlappingBoss = false;
	bool isUltimateActive = false;
	float att = 0.2;

	class ATengaiGameMode* tengaiGM;

	class AMidBoss* midBoss;
	class APreBoss* preBoss;
	class ABoss* boss;
};
