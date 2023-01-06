#include "ItemWidget.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"


void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UItemWidget::PrintGetItemInfo(ItemType type)
{
	switch (type)
	{
	case ItemType::POWER_UP:
		textBlock->SetText(FText::FromString(TEXT("POWER UP!")));
		break;
	case ItemType::ULTIMATE:
		textBlock->SetText(FText::FromString(TEXT("ULTIMATE!")));
		break;
	}
}

void UItemWidget::PrintMonsterScore(int32 score)
{
	FString str = FString(TEXT("+ ")) + FString::FromInt(score);
	textBlock->SetText(FText::FromString(str));
	textBlock->SetRenderScale(FVector2D(1.5, 1.5));
}
