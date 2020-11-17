// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBTDecorator.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyBTDecorator::UMyBTDecorator()
{
	NodeName = TEXT("CanAttack");
}

bool UMyBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr) return false;

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (Target == nullptr) return false;

	bResult = Target->GetDistanceTo(ControllingPawn) <= 200.0f ? true:false;

	return bResult;
}
