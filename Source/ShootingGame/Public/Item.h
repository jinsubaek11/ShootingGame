// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SHOOTINGGAME_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemSettings)
		class UBoxComponent* boxComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ItemSettings)
		class UStaticMeshComponent* meshComp;

	// 오버랩 감지 함수
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// 오버라이드 할 가상 함수
	virtual void ItemSelector(class APlayerFlight* player);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	FVector randomDir;

private:
	float itemSpeed = 200;

};
