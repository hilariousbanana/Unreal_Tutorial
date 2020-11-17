// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "Components/ActorComponent.h"
#include "MyActorComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPIsZeroDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PRACTICE_1027_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;		
	virtual void InitializeComponent() override;

public:
	void SetNewLevel(int32 NewLevel);
	void SetDamage(float NewDamage);
	float GetAttack();
	void SetHP(float NewHP);
	float GetHPRatio();

public:
	FOnHPIsZeroDelegate OnHpIsZero;
	FOnHPChangeDelegate OnHPChange;

private:
	struct FMyCharacterData* CurrentStatData = nullptr;
	UPROPERTY(EditInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true, DisplayPriority = 0))
		int32 Level;
	UPROPERTY(Transient, VisibleInstanceOnly, Category = "Stat", meta = (AllowPrivateAccess = true))
		float CurrentHP; 
};
