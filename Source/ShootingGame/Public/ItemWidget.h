#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item.h"
#include "ItemWidget.generated.h"


UCLASS()
class SHOOTINGGAME_API UItemWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct();

	void PrintGetItemInfo(ItemType type);
	void PrintMonsterScore(int32 score);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MySetting", meta = (BindWidget))
	class UTextBlock* textBlock;
};
