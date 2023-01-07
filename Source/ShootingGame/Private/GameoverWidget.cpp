// Fill out your copyright notice in the Description page of Project Settings.


#include "GameoverWidget.h"
#include "kismet/KismetSystemLibrary.h"
#include "kismet/GameplayStatics.h"
#include "components/Button.h"
#include "EndingWidget.h"
#include "TengaiGameMode.h"

void UGameoverWidget::NativeConstruct()
{
	btn_Resume->OnClicked.AddDynamic(this, &UGameoverWidget::ResumeGame);
	btn_Quit->OnClicked.AddDynamic(this, &UGameoverWidget::QuitGame);
}

void UGameoverWidget::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	this->RemoveFromParent();
}

void UGameoverWidget::QuitGame()
{
	ATengaiGameMode* gm = Cast<ATengaiGameMode>(GetWorld()->GetAuthGameMode());
	FString scoreText;
	FString filePath = gm->filePath;
	FFileHelper::LoadFileToString(scoreText, *filePath);
	int32 newBestScore = FCString::Atoi(*scoreText);
	gm->SetBestScore(newBestScore);
	endingUI = CreateWidget<UEndingWidget>(GetWorld(), endingWidget);

	if (endingUI != nullptr)
	{
		endingUI->AddToViewport();
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}
