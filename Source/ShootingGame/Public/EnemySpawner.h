// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class SHOOTINGGAME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	// 시간이 지날수록 증가하는 값을 받을 변수
	float currentTime;
	// 세팅 시간이 되면 적이 스폰되게끔
// 	UPROPERTY(EditInstanceOnly, Category=EnemySettings)
// 	float spawnTime=2;
	// 스폰할 블루프린트 선택창 만들기
	UPROPERTY(EditInstanceOnly, Category=EnemySettings)
	TSubclassOf <class AEnemy> enemyFactory;

	// 애로우 붙이기 위해 씬과 함께 선언
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	class UArrowComponent* spawnArrow;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	class USceneComponent* rootScene;

	FVector curLoc;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY1 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY2 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY3 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY4 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY5 = 99999;
<<<<<<< HEAD
=======
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY6 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY7 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY8 = 99999;
>>>>>>> b95a99b32cacab323193a55547cdd984304772fe

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
