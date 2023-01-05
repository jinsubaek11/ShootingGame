// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PrintCurrentScore();
	void PrintLifeCount();
	void PrintUltimateCount();

public:		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting", meta = (BindWidget))
	class UTextBlock* currentScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting", meta = (BindWidget))
	class UProgressBar* healthBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting", meta = (BindWidget))
	class UProgressBar* ultimateBar;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting", meta = (BindWidget))
	//class UWrapBox* lifeCountWrapBox;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting", meta = (BindWidget))
	//class UWrapBox* ultimateCountWrapBox;


private:
	bool isCreatedHeart = false;
};
