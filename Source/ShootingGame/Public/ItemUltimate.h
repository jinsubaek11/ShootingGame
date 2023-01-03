// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "ItemUltimate.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API AItemUltimate : public AItem
{
	GENERATED_BODY()
	
public:
	virtual void ItemSelector() override;
};
