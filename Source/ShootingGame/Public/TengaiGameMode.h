// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TengaiGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTINGGAME_API ATengaiGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	int32 GetCurrentScore();
	int32 GetBestScore();
	void SetBestScore(int32 newScore);
	void ShowGameover();
	void AddScore(int32 point);


public:
	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UMainWidget> mainWidget;

	//UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	//TSubclassOf<class UItemWidget> itemWidget;

	class UMainWidget* mainUI;
	//class UItemWidget* itemUI;
	
	float playSpeed = 50;

	FString filePath = FString("../../../Content/Savedata/BestScore.txt");
private:
	int32 currentScore = 0;
	int32 bestScore = 0;

	class UGameoverWidget* gameoverUI;
	UPROPERTY(EditAnywhere, Category = MyDefaultSettings)
	TSubclassOf <class UGameoverWidget> gameoverWidget;

};
