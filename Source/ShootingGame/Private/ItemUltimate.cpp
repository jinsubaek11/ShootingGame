// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUltimate.h"
#include "PlayerFlight.h"
#include "TengaiGameMode.h"

AItemUltimate::AItemUltimate()
{
	type = ItemType::ULTIMATE;
}

void AItemUltimate::BeginPlay()
{
	Super::BeginPlay();
}

void AItemUltimate::ItemSelector(APlayerFlight* player)
{
	if (player != nullptr)
	{
		int32 ultCount = player->GetUltimateCount();
		int32 maxUltCount = player->GetMaxUltimateCount();
		if (ultCount < maxUltCount)
		{
			player->AddUltimateCount();
			ATengaiGameMode* gm = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
			gm->mainUI->PrintUltimateCount();
		}
	}
}
