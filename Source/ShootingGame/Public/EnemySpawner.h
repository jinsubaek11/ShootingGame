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

	// �ð��� �������� �����ϴ� ���� ���� ����
	//float currentTime;

	//UPROPERTY(EditInstanceOnly, Category=EnemySettings)
	//float spawnTime=2;

	UPROPERTY(EditInstanceOnly, Category=EnemySettings)
	TSubclassOf <class AEnemy> enemyFactory;

	// �ַο� ���̱� ���� ���� �Բ� ����
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	class UArrowComponent* spawnArrow;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	class USceneComponent* rootScene;

	//FVector curLoc;
	float worldTime = 0;

	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer1 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer2 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer3 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer4 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer5 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer6 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer7 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnTimer8 = 99999;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
