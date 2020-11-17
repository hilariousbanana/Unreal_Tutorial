// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"


USTRUCT(BlueprintType)
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FMyCharacterData() : Level(1), MaxHP(100.0f), MaxMP(100.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
		int32 Level;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
		float MaxHP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
		float MaxMP;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
		int32 DropExp;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Data")
		int32 NextExp;
};

/**
 * 
 */
UCLASS()
class PRACTICE_1027_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	virtual void Init() override;
	FMyCharacterData* GetMyCharacterData(int32 Level);

private:
	UPROPERTY()
		class UDataTable* MyCharacterTable;
};
