// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameoverWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UGameoverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, meta = (bindwidget))
	class UButton* btn_Resume;
	UPROPERTY(EditDefaultsOnly, meta = (bindwidget))
	class UButton* btn_Quit;

	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void ResumeGame();
	UFUNCTION()
	void QuitGame();

};
