// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "MyWeapon.h"
#include "MyActorComponent.h"
#include "Components/WidgetComponent.h"
#include "MyUserWidget.h"
#include "MyAIController.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	GetCapsuleComponent()->SetCapsuleHalfHeight(88.0f);
	GetCapsuleComponent()->SetCapsuleRadius(34.0f);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	//SkeletalMesh'/Game/MyContents/Character/Mesh/Ch14_nonPBR.Ch14_nonPBR'
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_MOUSE(TEXT("/Game/MyContents/Character/Mesh/Ch14_nonPBR.Ch14_nonPBR"));
	
	if (SK_MOUSE.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_MOUSE.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//AnimBlueprint'/Game/MyContents/Character/NewAnimBlueprint.NewAnimBlueprint'
	static ConstructorHelpers::FClassFinder<UAnimInstance>
		MOUSE_ANIM(TEXT("/Game/MyContents/Character/NewAnimBlueprint.NewAnimBlueprint_C"));

	if (MOUSE_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(MOUSE_ANIM.Class);
	}

	SetControlMode(EControlMode::DIABLO);

	ArmLengthSpeed = 3.0f;
	ArmRotationSpeed = 10.0f;

	GetCharacterMovement()->JumpZVelocity = 800.0f;

	IsAttacking = false;

	MaxCombo = 3;
	AttackEndComboState();

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));

	attackRange = 200.0f;
	attackRadius = 50.0f;

	CharacterStat = CreateDefaultSubobject<UMyActorComponent>(TEXT("CHARACTERSTAT"));

	HPBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBARWIDGET"));
	HPBarWidget->SetupAttachment(GetMesh());

	HPBarWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	HPBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	//WidgetBlueprint'/Game/MyContents/ui/UI_HPBar.UI_HPBar'
	static ConstructorHelpers::FClassFinder<UUserWidget> UI_HUD(TEXT("/Game/MyContents/ui/UI_HPBar.UI_HPBar_C"));
	if (UI_HUD.Succeeded())
	{
		HPBarWidget->SetWidgetClass(UI_HUD.Class);
		HPBarWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	}

	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//DamageTextWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DAMAGEWIDGET"));
	//DamageTextWidget->SetupAttachment(GetMesh());

	//DamageTextWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 190.0f));
	//DamageTextWidget->SetWidgetSpace(EWidgetSpace::Screen);

	////WidgetBlueprint'/Game/MyContents/ui/UI_Damage.UI_Damage'
	//static ConstructorHelpers::FClassFinder<UUserWidget> UI_TEXT(TEXT("/Game/MyContents/ui/UI_Damage.UI_Damage_C"));
	//if (UI_TEXT.Succeeded())
	//{
	//	DamageTextWidget->SetWidgetClass(UI_TEXT.Class());
	//}
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	auto CharacterWidget = Cast<UMyUserWidget>(HPBarWidget->GetUserWidgetObject());
	if (CharacterWidget != nullptr)
	{
		CharacterWidget->BindCharacterStat(CharacterStat);
	}
}

void AMyCharacter::SetControlMode(EControlMode newControlMode)
{
	CurrentControlMode = newControlMode;
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		//SpringArm->TargetArmLength = 450.0f;
		//SpringArm->SetRelativeRotation(FRotator::ZeroRotator);
		ArmLengthTo = 450.0f;
		SpringArm->bUsePawnControlRotation = true;
		SpringArm->bInheritPitch = true;
		SpringArm->bInheritRoll = true;
		SpringArm->bInheritYaw = true;
		SpringArm->bDoCollisionTest = true;
		bUseControllerRotationYaw = false;
		//이동방향으로 회전
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::DIABLO:
		//SpringArm->TargetArmLength = 800.0f;
		//SpringArm->SetRelativeRotation(FRotator(-45.0f,0.0f,0.0f));
		ArmLengthTo = 800.0f;
		ArmRotationTo = FRotator(-45.0f, 0.0f, 0.0f);
		SpringArm->bUsePawnControlRotation = false;
		SpringArm->bInheritPitch = false;
		SpringArm->bInheritRoll = false;
		SpringArm->bInheritYaw = false;
		SpringArm->bDoCollisionTest = false;
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
		break;
	case EControlMode::NPC:
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bUseControllerDesiredRotation = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
		break;
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, ArmLengthTo, DeltaTime, ArmLengthSpeed);

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		SpringArm->SetRelativeRotation
		(FMath::RInterpTo(SpringArm->GetRelativeRotation(), ArmRotationTo, DeltaTime, ArmRotationSpeed));
		break;
	}

	switch (CurrentControlMode)
	{
	case EControlMode::DIABLO:
		if (DirectionToMove.SizeSquared() > 0.0f)
		{
			GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator());
			AddMovementInput(DirectionToMove);
		}
		break;
	}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (MyAnim != nullptr)
	{
		MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded); //몽타주 애니메이션이 끝났을 때 호출.
	}

	MyAnim->OnNextAttackCheck.AddLambda([this]() -> void 
		{
			ALOG(Warning, TEXT("OnNextAttackCheck"));
			CanNextCombo = true;
			//if (IsComboInputOn)
			//{
			//	AttackStartComboState();
			//	MyAnim->JumpToAttackMontageSection(CurrentCombo);
			//}
		});
		
	MyAnim->OnComboCheck.AddLambda([this]()->void
		{
			if (IsComboInputOn >= 1)
			{
				//success
				CurrentCombo++;
				MyAnim->SetNextSection(CurrentCombo);
			}

			CanNextCombo = false;
			IsComboInputOn = 0;
		});

	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);
	
	CharacterStat->OnHpIsZero.AddLambda([this]()->void
		{
			MyAnim->SetDeadAnim();
			SetActorEnableCollision(false);
		});
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);

	PlayerInputComponent->BindAction(TEXT("ChangeView"),
		EInputEvent::IE_Pressed, this, &AMyCharacter::ChangeView);

	PlayerInputComponent->BindAction(TEXT("Jump"),
		EInputEvent::IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction(TEXT("Attack"),
		EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
}

float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	//ALOG(Warning, TEXT("Actor : %s took Damage : %f"), *GetName(), FinalDamage);
	
	CharacterStat->SetDamage(FinalDamage);
	return FinalDamage;
}

bool AMyCharacter::CanSetWeapon()
{
	return CurrentWeapon == nullptr;
}

void AMyCharacter::SetWeapon(AMyWeapon * NewWeapon)
{
	if (NewWeapon != nullptr && CurrentWeapon == nullptr)
	{
		FName WeaponSocket(TEXT("RightHand_Socket"));
		NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocket);
		NewWeapon->SetOwner(this);
		CurrentWeapon = NewWeapon;
	}
}

void AMyCharacter::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

	if (IsPlayerControlled())
	{
		SetControlMode(EControlMode::DIABLO);
	}
	else
	{
		SetControlMode(EControlMode::NPC);
		GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	}
}

void AMyCharacter::UpDown(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::X), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		if (IsAttacking)
		{
			DirectionToMove.X = 0.0f;
			DirectionToMove.Y = 0.0f;
		}
		else
		{
			DirectionToMove.X = NewAxisValue;
		}
		break;
	}
}

void AMyCharacter::LeftRight(float NewAxisValue)
{
	if (IsAttacking) return;

	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddMovementInput(FRotationMatrix(GetControlRotation()).GetUnitAxis(EAxis::Y), NewAxisValue);
		break;
	case EControlMode::DIABLO:
		if (IsAttacking)
		{
			DirectionToMove.X = 0.0f;
			DirectionToMove.Y = 0.0f;
		}
		else
		{
			DirectionToMove.Y = NewAxisValue;
		}
		break;
	}
}

void AMyCharacter::Turn(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerYawInput(NewAxisValue);
		break;
	}
}

void AMyCharacter::LookUp(float NewAxisValue)
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		AddControllerPitchInput(NewAxisValue);
		break;
	}
}

void AMyCharacter::ChangeView()
{
	switch (CurrentControlMode)
	{
	case EControlMode::GTA:
		GetController()->SetControlRotation(GetActorRotation());
		SetControlMode(EControlMode::DIABLO);
		break;
	case EControlMode::DIABLO:
		GetController()->SetControlRotation(SpringArm->GetRelativeRotation());
		SetControlMode(EControlMode::GTA);
		break;
	}
}

void AMyCharacter::Attack()
{
	if (IsAttacking)
	{
		if (FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo))
		{
			if (CanNextCombo)
			{
				IsComboInputOn++;
			}
		}
	}
	else
	{
		if (CurrentCombo == 0)
		{
			CurrentCombo = 1;
			//AttackStartComboState();
			MyAnim->PlayAttackMontage();
			//MyAnim->JumpToAttackMontageSection(CurrentCombo);
			IsAttacking = true;
		}

	}
}

void AMyCharacter::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * attackRange, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(attackRadius),
		Params);

#if ENABLE_DRAW_DEBUG

	FVector TraceVec = GetActorForwardVector() * attackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = attackRange * 0.5f + attackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 5.0f;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, attackRadius, CapsuleRot, DrawColor, false, DebugLifeTime);

#endif

	if (bResult)
	{
		if (HitResult.Actor.IsValid())
		{
			//ALOG(Warning, TEXT("Hit Actor : %s"), *HitResult.Actor->GetName());
			FDamageEvent DamageEvent;
			HitResult.Actor->TakeDamage(CharacterStat->GetAttack(), DamageEvent, GetController(), this);
		}
	}
}

void AMyCharacter::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	if (IsAttacking)
	{
		if (CurrentCombo > 0)
		{
			IsAttacking = false;
			AttackEndComboState();
			OnAttackEnd.Broadcast();
		}
	}
}

void AMyCharacter::AttackStartComboState()
{
	CanNextCombo = true;
	IsComboInputOn = false;
	if (FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1))
	{
		CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
	}
}

void AMyCharacter::AttackEndComboState()
{
	IsComboInputOn = false;
	CanNextCombo = false;
	CurrentCombo = 0;
}

void AMyCharacter::ComboCheck()
{
	if (IsComboInputOn)
	{
		//success
		CurrentCombo++;
		MyAnim->SetNextSection(CurrentCombo);
	}

	CanNextCombo = false;
	IsComboInputOn = false;
}

