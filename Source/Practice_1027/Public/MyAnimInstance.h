// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnComboCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
/**
 * 
 */
UCLASS()
class PRACTICE_1027_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
		bool IsInAir;
	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		UAnimMontage* AttackMontage;
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Pawn", meta = (AllowPrivateAccess = true))
		bool IsDead;
private:
	UFUNCTION()
		void AnimNotify_AttackHitCheck(); //함수 이름: AnimNotify_Notify이름
	UFUNCTION()
		void AnimNotify_AttackEnd();
	UFUNCTION()
		void AnimNotify_ComboCheck();
	FName GetAttackMontageSectionName(int32 Section);

public:
	FOnNextAttackCheckDelegate OnNextAttackCheck;
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	FOnComboCheckDelegate OnComboCheck;

public:
	UMyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void JumpToAttackMontageSection(int32 NewSection);
	void SetNextSection(int32 NewSection);
	void SetDeadAnim()
	{
		IsDead = true;
	}
};
