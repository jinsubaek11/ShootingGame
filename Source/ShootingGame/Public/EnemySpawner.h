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
	float currentTime;

	UPROPERTY(EditInstanceOnly, Category=EnemySettings)
	TSubclassOf <class AEnemy> enemyFactory;

	// �ַο� ���̱� ���� ���� �Բ� ����
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
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY6 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY7 = 99999;
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	float spawnY8 = 99999;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
