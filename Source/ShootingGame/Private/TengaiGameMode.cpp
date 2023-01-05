// Fill out your copyright notice in the Description page of Project Settings.


#include "TengaiGameMode.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine.h"
#include "MidBoss.h"
#include "kismet/GameplayStatics.h"
#include "GameoverWidget.h"

void ATengaiGameMode::BeginPlay()
{
	GEngine->GetGameUserSettings()->SetScreenResolution(FIntPoint(1920, 1080));
	GEngine->GetGameUserSettings()->SetFrameRateLimit(60);
}

void ATengaiGameMode::Tick(float DeltaTime)
{

}

void ATengaiGameMode::ShowGameover()
{
	gameoverUI = CreateWidget<UGameoverWidget>(GetWorld(), gameoverWidget);

	if (gameoverUI != nullptr)
	{
		gameoverUI->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}
