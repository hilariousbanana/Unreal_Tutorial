// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMyAIController::HomePosKey(TEXT("HomePos"));
const FName AMyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMyAIController::TargetKey(TEXT("Target"));

AMyAIController::AMyAIController()
{
	//RepeatInterval = 3.0f;
	//BlackboardData'/Game/MyContents/BB_Character.BB_Character'
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/MyContents/BB_Character.BB_Character"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}
	
	//BehaviorTree'/Game/MyContents/BT_Character.BT_Character'
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/MyContents/BT_Character.BT_Character"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AMyAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	//GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AMyAIController::OnRepeatTimer, RepeatInterval, true);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation()); //현재 pawn의 위치

		if (!RunBehaviorTree(BTAsset))
		{
			ALOG(Error, TEXT("AIController couldn't run behavior tree."));
		}
	}
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	//GetWorld()->GetTimerManager().ClearTimer(RepeatTimerHandle);
}

//void AMyAIController::OnRepeatTimer()
//{
//	auto CurrentPawn = GetPawn();
//	if (CurrentPawn == nullptr) return;
//
//	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld()); //현재 네비게이션 시스템을 얻어옴.
//	if (NavSystem == nullptr) return;
//
//	FNavLocation NextLocation;
//	if (NavSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, NextLocation))
//	{
//		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, NextLocation.Location);
//	}
//}
