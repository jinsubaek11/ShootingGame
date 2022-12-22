// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SHOOTINGGAME_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

	// collision 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	class UBoxComponent* boxComp;
	// mesh 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	class UStaticMeshComponent* meshComp;

	// 총알속도 변수 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	float bulletSpeed=1600;
	// 총알이동 벡터변수 선언
	FVector direction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
