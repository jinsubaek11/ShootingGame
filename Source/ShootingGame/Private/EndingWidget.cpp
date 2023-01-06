// Fill out your copyright notice in the Description page of Project Settings.


#include "EndingWidget.h"
#include "Kismet/GameplayStatics.h"
#include "components/TextBlock.h"
#include "TengaiGameMode.h"
#include "components/Button.h"



void UEndingWidget::NativeConstruct()
{
	btn_Restart->OnClicked.AddDynamic(this, &UEndingWidget::RestartGame);
	btn_Quit->OnClicked.AddDynamic(this, &UEndingWidget::QuitGame);
	PrintCurScore();
}

void UEndingWidget::PrintCurScore()
{
	ATengaiGameMode* myGM = Cast<ATengaiGameMode>(UGameplayStatics::GetGameMode(this));
	if (myGM != nullptr)
	{
		curScore->SetText(FText::AsNumber(myGM->GetCurrentScore()));
		bestScore->SetText(FText::AsNumber(myGM->GetBestScore()));
	}
}

void UEndingWidget::RestartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartLevel"));
	this->RemoveFromParent();
}

void UEndingWidget::QuitGame()
{
	APlayerController* playCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), playCon, EQuitPreference::Quit, true);
}
