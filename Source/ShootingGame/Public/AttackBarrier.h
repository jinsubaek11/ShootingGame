// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttackBarrier.generated.h"

UCLASS()
class SHOOTINGGAME_API AAttackBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttackBarrier();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = PlayerSettings)
	class UArrowComponent* arrowComp;

	// 어택배리어에서 발사되는 총알의 BP 경로 지정
	TSoftClassPtr<AActor> subBulletBP = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("Blueprint'/Game/Blueprints/BP_Bullet.BP_Bullet_C'")));

private:
	float delay = 3.f;
	float accTime = 0.f;
	float accRotation = 0.f;
	float distance = 150.f;
};
