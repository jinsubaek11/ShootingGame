// Fill out your copyright notice in the Description page of Project Settings.


#include "MainWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/WrapBox.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "TengaiGameMode.h"
#include "PlayerFlight.h"
#include "Blueprint/WidgetTree.h"



void UMainWidget::PrintCurrentScore()
{
	ATengaiGameMode* gameMode = Cast<ATengaiGameMode>(UGameplayStatics::GetGameMode(this));

	if (gameMode)
	{
		currentScore->SetText(FText::AsNumber(gameMode->GetCurrentScore()));
	}
}

void UMainWidget::PrintLifeCount()
{
	APlayerFlight* player = Cast<APlayerFlight>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (player)
	{
		int32 lifeCount = player->GetLifeCount();
		int32 maxLifeCount = player->GetMaxLifeCount();

		healthBar->SetPercent(lifeCount / (float)maxLifeCount);
	}

	//if (player)
	//{
	//	int8 lifeCount = player->GetLifeCount();

	//	if (!isCreatedHeart)
	//	{
	//		for (int i = 0; i < lifeCount; i++)
	//		{
	//			UImage* heartImg = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("HeartImage"));
	//			heartImg->SetBrushFromTexture(LoadObject<UTexture2D>(nullptr, TEXT("/Script/Engine.Texture2D'/Game/Asstets/Widget/heart2.heart2'")));
	//			lifeCountWrapBox->AddChild(heartImg);
	//		}
	//		UE_LOG(LogTemp, Warning, TEXT("create"));

	//		isCreatedHeart = true;
	//	}

	//	//UE_LOG(LogTemp, Warning, TEXT("%d"), lifeCount);

	//	for (int i = 0; i < lifeCountWrapBox->GetAllChildren().Num(); i++)
	//	{
	//		//UE_LOG(LogTemp, Warning, TEXT("%d"), lifeCountWrapBox->GetAllChildren().Num());
	//		ESlateVisibility visibility = lifeCount <= i ? ESlateVisibility::Hidden : ESlateVisibility::Visible;
	//		lifeCountWrapBox->GetAllChildren()[i]->SetVisibility(visibility);

	//		UE_LOG(LogTemp, Warning, TEXT("Visiblility: %s\tlife count: %d"), lifeCount <= i ? TEXT("True") : TEXT("False"), lifeCount);
	//		//UE_LOG(LogTemp, Warning, TEXT("%d"), i);

	//		//lifeCountWrapBox->GetAllChildren()[2]->SetVisibility(ESlateVisibility::Hidden);
	//	}
	//}
}

void UMainWidget::PrintUltimateCount()
{
	APlayerFlight* player = Cast<APlayerFlight>(UGameplayStatics::GetPlayerPawn(this, 0));

	if (player)
	{
		int32 ultimateCount = player->GetUltimateCount();
		int32 ultimateMaxCount = player->GetMaxUltimateCount();

		ultimateBar->SetPercent(ultimateCount / (float)(ultimateMaxCount + 1));
	}
}
