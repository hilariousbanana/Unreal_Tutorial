// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class PRACTICE_1027_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	enum class EControlMode
	{
		GTA,
		DIABLO,
		NPC
	};

	EControlMode CurrentControlMode = EControlMode::GTA;
	FVector DirectionToMove = FVector::ZeroVector;
	float ArmLengthTo = 0.0f;
	FRotator ArmRotationTo = FRotator::ZeroRotator;
	float ArmLengthSpeed = 0.0f;
	float ArmRotationSpeed = 0.0f;

public:
	// Sets default values for this character's properties
	AMyCharacter();

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SetControlMode(EControlMode newControlMode);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override; //initialize 전에 미리 호출

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
		AActor* DamageCauser) override;

	bool CanSetWeapon();
	void SetWeapon(class AMyWeapon* NewWeapon);

	virtual void PossessedBy(AController* NewController) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		USkeletalMeshComponent* Weapon;
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		class AMyWeapon* CurrentWeapon;
	UPROPERTY(VisibleAnywhere, Category = "Stat")
		class UMyActorComponent* CharacterStat;
	UPROPERTY(VisibleAnywhere, Category = "UI")
		class UWidgetComponent* HPBarWidget;
	/*UPROPERTY(VisibleAnywhere, Category = "UI")
		class UWidgetComponent* DamageTextWidget;*/

private:
	void UpDown(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	void Turn(float NewAxisValue);
	void LookUp(float NewAxisValue);

	void ChangeView();


	void AttackCheck();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted); //몽타주 애니메이션이 끝났을 때 호출 될 함수. delegate 함수 이므로 parameter는 반드시 동일.
	void AttackStartComboState();
	void AttackEndComboState();

	void ComboCheck();

private:
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY()
		class UMyAnimInstance* MyAnim;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		bool CanNextCombo;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		int32 IsComboInputOn;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		int32 MaxCombo;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		float attackRange;
	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, Category = "Attack", meta = (AllowPrivateAccess = true))
		float attackRadius;
}; 
