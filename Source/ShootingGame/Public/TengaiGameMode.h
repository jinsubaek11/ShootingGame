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
	virtual void Tick(float DeltaTime) override;

public:
	int32 GetCurrentScore();
	void AddScore(int32 point);
	void ShowGameover();

public:
	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UMainWidget> mainWidget;

	UPROPERTY(EditAnywhere, Category = MyDefaultSetting)
	TSubclassOf<class UHPWidget> bossHPWidget;

	class UMainWidget* mainUI;
	class UHPWidget* bossUI;
	
	float playSpeed = 50;
	bool encountMidBoss = true;

private:
	int32 currentScore = 0;

	class UGameoverWidget* gameoverUI;
	UPROPERTY(EditAnywhere, Category = MyDefaultSettings)
	TSubclassOf <class UGameoverWidget> gameoverWidget;
};
