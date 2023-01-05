// Fill out your copyright notice in the Description page of Project Settings.


#include "TengaiGameMode.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine.h"
#include "MainWidget.h"
#include "ItemWidget.h"

void ATengaiGameMode::BeginPlay()
{
	GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1920, 1080));
	GEngine->GetGameUserSettings()->SetFrameRateLimit(60);

	mainUI = CreateWidget<UMainWidget>(GetWorld(), mainWidget);
	//itemUI = CreateWidget<UItemWidget>(GetWorld(), itemWidget);

	if (mainUI)
	{
		mainUI->AddToViewport();
		mainUI->PrintCurrentScore();
		mainUI->PrintLifeCount();
		mainUI->PrintUltimateCount();
	}

	//if (itemUI)
	//{
	//	//itemUI->AddToViewport();
	//}
}

void ATengaiGameMode::Tick(float DeltaTime)
{

}

int32 ATengaiGameMode::GetCurrentScore()
{
	return currentScore;
}
