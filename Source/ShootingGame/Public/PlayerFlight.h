// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerFlight.generated.h"

#define WEAK 0b00000001
#define NORMAL 0b00000010
#define STRONG 0b00000100


UCLASS()
class SHOOTINGGAME_API APlayerFlight : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerFlight();

	// Collision 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	class UBoxComponent* boxcomp;
	// Mesh 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	class UStaticMeshComponent* meshcomp;

	// 이동을 위한 벡터 선언
	FVector direction;
	// 이동속도 변수 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	float movespeed = 800;

	// 총알 블루프린트를 넣을 수 있는 변수 선언
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	TSubclassOf<class ABullet>bulletfactory;

	UPROPERTY(EditDefaultsOnly, Category = PlayerSettings)
	float shootingDelay = 0.1;

	UPROPERTY(EditAnywhere, Category = PlayerSettings)
	uint8 attackLevel = 0;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void SetAttackLevel(char value);

private:
	// 좌우입력이 들어왔을 때 실행될 함수 선언
	void HorizontalInput(float value);
	// 상하입력이 들어왔을 때 실행될 함수 선언
	void VerticalInput(float value);
	// 클릭입력이 들어왔을 때 실행될 함수 선언
	void bulletfire();

	void Fire(float value);

private:
	UPROPERTY()
	TArray<class ABullet*> projectiles;

	float accTime = 0;
	bool isShoot = false;

	const uint8 MIN_DEGREE = -5;
	const uint8 MAX_DEGREE =  5;
	const uint8 COUNT_CONTROL_VAR = 4;
};

