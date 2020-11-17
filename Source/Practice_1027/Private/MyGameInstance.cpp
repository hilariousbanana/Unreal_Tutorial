// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

UMyGameInstance::UMyGameInstance()
{
	//DataTable'/Game/MyContents/Data/assetsavefile.assetsavefile'
	FString CharacterDataPath = TEXT("/Game/MyContents/Data/assetsavefile.assetsavefile");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_CHARACTER(*CharacterDataPath);
	if (DT_CHARACTER.Succeeded())
	{
		MyCharacterTable = DT_CHARACTER.Object;
	}
}

void UMyGameInstance::Init()
{
	Super::Init();
	ALOG(Warning, TEXT("DropExp of Level 20 character : %d"), GetMyCharacterData(20)->DropExp);
}

FMyCharacterData * UMyGameInstance::GetMyCharacterData(int32 Level)
{
	return MyCharacterTable->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}
