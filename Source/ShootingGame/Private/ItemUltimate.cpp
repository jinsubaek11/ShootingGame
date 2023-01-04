// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUltimate.h"
#include "PlayerFlight.h"

void AItemUltimate::BeginPlay()
{
	Super::BeginPlay();


}

void AItemUltimate::ItemSelector(APlayerFlight* player)
{
	UE_LOG(LogTemp, Warning, TEXT("This is Ultimate item"));
}
