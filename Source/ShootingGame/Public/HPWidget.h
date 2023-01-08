#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPWidget.generated.h"


UCLASS()
class SHOOTINGGAME_API UHPWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void PrintCurrentHealth(float currentHP, float MaxHP, FLinearColor color = FLinearColor::Red);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting", meta = (BindWidget))
	class UProgressBar* healthBar;
};
