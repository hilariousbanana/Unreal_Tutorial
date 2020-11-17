// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_1027_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void BindCharacterStat(class UMyActorComponent* NewDataStat);

private:
	TWeakObjectPtr<class UMyActorComponent> CurrentCharacterStat;
	UPROPERTY()
		class UProgressBar* HPProgressBar;

protected:
	virtual void NativeConstruct() override;
	void UpdateHPWidget();
};
