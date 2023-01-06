// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UEndingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySettings|Score", meta = (BindWidget))
	class UTextBlock* curScore;

	UPROPERTY(EditAnywhere, Category = "MySettings|Score", meta = (BindWidget))
	class UTextBlock* bestScore;

	UPROPERTY(EditDefaultsOnly, meta = (bindwidget))
	class UButton* btn_Restart;

	UPROPERTY(EditDefaultsOnly, meta = (bindwidget))
	class UButton* btn_Quit;

	void PrintCurScore();
	UFUNCTION()
	void RestartGame();
	UFUNCTION()
	void QuitGame();
};
