// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTINGGAME_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

	// 박스 콜리전 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UBoxComponent* boxComp;
	// 메쉬 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UStaticMeshComponent* meshComp;

	// 적 이동 속도 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	float enemySpeed=400;
	// 적 이동 벡터
	FVector direction;
	// 이동 모드 변수
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	int32 movingMode=1;
	float runningTime=0;

	// 아이템 블루프린트 선택창 만들기
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	TSubclassOf <class AItem> itemFactory;

	// 오버랩 감지 함수
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 아이템 드롭 확률
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float dropRate=0.5;
	float drawRate;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
