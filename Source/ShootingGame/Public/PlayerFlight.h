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

	// Collision ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	class UBoxComponent* boxComp;
	// Mesh ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	class UStaticMeshComponent* meshComp;

	// �̵��� ���� ���� ����
	FVector direction;
	// �̵��ӵ� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	float moveSpeed = 800;

	// �Ѿ� ��������Ʈ�� ���� �� �ִ� ���� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category=PlayerSettings)
	TSubclassOf<class ABullet>bulletFactory;

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
	// �¿��Է��� ������ �� ����� �Լ� ����
	void HorizontalInput(float value);
	// �����Է��� ������ �� ����� �Լ� ����
	void VerticalInput(float value);
	// Ŭ���Է��� ������ �� ����� �Լ� ����
	void BulletFire();

	void Fire(float value);

private:
	UPROPERTY()
	TArray<class ABullet*> projectiles;
	TArray<class AAttackBarrier*> attackBarriers;

	float accTime = 0;
	bool isShoot = false;

	const int8 MIN_DEGREE = -5;
	const int8 MAX_DEGREE =  5;
	const int8 COUNT_CONTROL_VAR = 4;
};

