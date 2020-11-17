// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorComponent.h"
#include "MyGameInstance.h"

// Sets default values for this component's properties
UMyActorComponent::UMyActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bWantsInitializeComponent = true;

	Level = 1;

}


// Called when the game starts
void UMyActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMyActorComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetNewLevel(Level);
}

void UMyActorComponent::SetNewLevel(int32 NewLevel)
{
	auto myGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())); //현재 사용하고 있는 게임인스턴스를 가져와서 형변환 한 것.

	if (myGameInstance == nullptr) return;

	CurrentStatData = myGameInstance->GetMyCharacterData(NewLevel);

	if (CurrentStatData == nullptr)
	{
		ALOG(Warning, TEXT("Level(%d) data doesn't exist."), NewLevel);
		return;

	}
	else
	{
		Level = NewLevel;
		SetHP(CurrentStatData->MaxHP);
	}
}

void UMyActorComponent::SetDamage(float NewDamage)
{
	if (CurrentStatData == nullptr) return;
	SetHP(FMath::Clamp<float>(CurrentHP - NewDamage, 0.0f, CurrentStatData->MaxHP));
}

float UMyActorComponent::GetAttack()
{
	if (CurrentStatData == nullptr) return 0.0f;
	//return CurrentStatData->MaxHP; //ATtack추가해야함ㅋㅋ
	return 10.0f;
}

void UMyActorComponent::SetHP(float NewHP)
{
	CurrentHP = NewHP;
	OnHPChange.Broadcast();
	if (CurrentHP < KINDA_SMALL_NUMBER)
	{
		CurrentHP = 0.0f;
		OnHpIsZero.Broadcast();
	}
}

float UMyActorComponent::GetHPRatio()
{
	if (CurrentStatData == nullptr) return 0.0f;

	return CurrentStatData->MaxHP < KINDA_SMALL_NUMBER ? 0.0f : CurrentHP/CurrentStatData->MaxHP;
}


