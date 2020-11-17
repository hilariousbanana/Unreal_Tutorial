// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnBTTaskNode.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UTurnBTTaskNode::UTurnBTTaskNode()
{
	NodeName = TEXT("Turn");
}

EBTNodeResult::Type UTurnBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto myCharacter = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (myCharacter == nullptr) return EBTNodeResult::Failed;

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (Target == nullptr) return EBTNodeResult::Failed;

	FVector LookVector = Target->GetActorLocation() - myCharacter->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	myCharacter->SetActorRotation(FMath::RInterpTo(myCharacter->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 2.0f));

	return EBTNodeResult::Succeeded;
}