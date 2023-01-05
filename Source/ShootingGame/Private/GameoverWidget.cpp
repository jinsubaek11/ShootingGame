// Fill out your copyright notice in the Description page of Project Settings.


#include "GameoverWidget.h"
#include "kismet/KismetSystemLibrary.h"
#include "kismet/GameplayStatics.h"
#include "components/Button.h"

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
	APlayerController* playCon = GetWorld()->GetFirstPlayerController();
	UKismetSystemLibrary::QuitGame(GetWorld(), playCon, EQuitPreference::Quit, true);
}
