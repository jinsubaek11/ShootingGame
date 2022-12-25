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

	// �ڽ� �ݸ��� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UBoxComponent* boxComp;
	// �޽� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=EnemySettings)
	class UStaticMeshComponent* meshComp;

	// �� �̵� �ӵ� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EnemySettings)
	float enemySpeed=400;
	// �� �̵� ����
	FVector direction;

	// ������ ��������Ʈ ����â �����
	UPROPERTY(EditInstanceOnly, Category = EnemySettings)
	TSubclassOf <class AItem> itemFactory;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};