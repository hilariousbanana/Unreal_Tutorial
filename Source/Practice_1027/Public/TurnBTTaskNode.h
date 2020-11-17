// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TurnBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_1027_API UTurnBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UTurnBTTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
