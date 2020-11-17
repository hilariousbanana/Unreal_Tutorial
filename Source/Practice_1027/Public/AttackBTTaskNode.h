// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AttackBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_1027_API UAttackBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UAttackBTTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool IsAttacking = false;
};
