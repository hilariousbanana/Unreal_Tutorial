// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "BehaviorTree/BTService.h"
#include "MyBTService.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_1027_API UMyBTService : public UBTService
{
	GENERATED_BODY()
public:
	UMyBTService();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory, float DeltaSeconds) override;
};
