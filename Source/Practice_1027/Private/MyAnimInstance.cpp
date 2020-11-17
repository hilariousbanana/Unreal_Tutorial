// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UMyAnimInstance::AnimNotify_AttackEnd()
{
	OnNextAttackCheck.Broadcast();
}

void UMyAnimInstance::AnimNotify_ComboCheck()
{
	OnComboCheck.Broadcast();
}

FName UMyAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	if (FMath::IsWithinInclusive<int32>(Section, 1, 3))
	{
		return FName(*FString::Printf(TEXT("Attack%d"), Section));
	}

	return NAME_None;
}

UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	IsInAir = false;
	//AnimMontage'/Game/MyContents/Character/AttackMontage.AttackMontage'
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/MyContents/Character/AttackMontage.AttackMontage"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
	IsDead = false;
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		if (IsDead) return;
		CurrentPawnSpeed = Pawn->GetVelocity().Size();

		auto Character = Cast<ACharacter>(Pawn);
		if (Character != nullptr)
		{
			IsInAir = Character->GetMovementComponent()->IsFalling();
		}
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	if (IsDead) return;
	Montage_Play(AttackMontage, 1.0f);
}

void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	if (IsDead) return;
	if (Montage_IsPlaying(AttackMontage))
	{
		//FName name = GetAttackMontageSectionName(NewSection);
		Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
	}
}

void UMyAnimInstance::SetNextSection(int32 NewSection)
{
	if (IsDead) return;
	if (Montage_IsPlaying(AttackMontage))
	{
		FName name = GetAttackMontageSectionName(NewSection);
		Montage_SetNextSection(Montage_GetCurrentSection(), name);
	}
}
