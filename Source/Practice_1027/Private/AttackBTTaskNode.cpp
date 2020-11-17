// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackBTTaskNode.h"
#include "MyAIController.h"
#include "MyCharacter.h"

UAttackBTTaskNode::UAttackBTTaskNode()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UAttackBTTaskNode::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto myCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (myCharacter == nullptr) return EBTNodeResult::Failed;

	myCharacter->Attack();
	IsAttacking = true;
	myCharacter->OnAttackEnd.AddLambda([this]()->void 
	{
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UAttackBTTaskNode::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
