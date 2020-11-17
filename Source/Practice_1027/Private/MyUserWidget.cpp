// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "MyActorComponent.h"
#include "Components/ProgressBar.h"

void UMyUserWidget::BindCharacterStat(class UMyActorComponent* NewDataStat)
{
	if (NewDataStat == nullptr) return;

	CurrentCharacterStat = NewDataStat;
	CurrentCharacterStat->OnHPChange.AddUObject(this, &UMyUserWidget::UpdateHPWidget);
}

void UMyUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PB_HPBAR")));
	if (HPProgressBar == nullptr) return;
	UpdateHPWidget();
}

void UMyUserWidget::UpdateHPWidget()
{
	if (CurrentCharacterStat.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(CurrentCharacterStat->GetHPRatio());
		}
	}
}
