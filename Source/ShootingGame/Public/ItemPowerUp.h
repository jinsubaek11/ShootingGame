// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AItemPowerUp : public AItem
{
	GENERATED_BODY()

public:
	virtual void ItemSelector(class APlayerFlight* player) override;
//	class APlayerFlight* player;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
