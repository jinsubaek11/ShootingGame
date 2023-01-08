#include "HPWidget.h"
#include "Components/ProgressBar.h"


void UHPWidget::PrintCurrentHealth(float currentHP, float MaxHP, FLinearColor color)
{
	healthBar->SetPercent(currentHP / MaxHP);
	healthBar->SetFillColorAndOpacity(color);
}