// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPowerUp.h"
#include "PlayerFlight.h"

void AItemPowerUp::BeginPlay()
{
	Super::BeginPlay();

}

void AItemPowerUp::ItemSelector(APlayerFlight* player)
{
		if (player != nullptr)
		{
			player->SetAttackLevel((AttackLevel)(player->GetAttackLevel() + 1));
		}
}
