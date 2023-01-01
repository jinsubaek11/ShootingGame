// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TengaiGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API ATengaiGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	float playSpeed=50;
};
