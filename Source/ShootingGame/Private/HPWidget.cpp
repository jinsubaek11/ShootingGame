#include "HPWidget.h"
#include "Components/ProgressBar.h"


void UHPWidget::PrintCurrentHealth(float currentHP, float MaxHP)
{
	healthBar->SetPercent(currentHP / MaxHP);
}