// Fill out your copyright notice in the Description page of Project Settings.


#include "TengaiGameMode.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine.h"
#include "MidBoss.h"

void ATengaiGameMode::BeginPlay()
{
	GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1920, 1080));
	GEngine->GetGameUserSettings()->SetFrameRateLimit(60);
}

void ATengaiGameMode::Tick(float DeltaTime)
{

}
