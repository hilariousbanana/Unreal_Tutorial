// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class PRACTICE_1027_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

//private:
//	void OnRepeatTimer();
//
//private:
//	FTimerHandle RepeatTimerHandle;
//	float RepeatInterval;
//
private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;
};
