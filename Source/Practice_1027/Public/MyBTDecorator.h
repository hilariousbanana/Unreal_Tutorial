// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "BehaviorTree/BTDecorator.h"
#include "MyBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_1027_API UMyBTDecorator : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UMyBTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
