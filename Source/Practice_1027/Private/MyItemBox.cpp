// Fill out your copyright notice in the Description page of Project Settings.

#include "MyWeapon.h"
#include "MyCharacter.h"
#include "MyItemBox.h"


// Sets default values
AMyItemBox::AMyItemBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));
	Box = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BOX"));

	RootComponent = Trigger;
	Box->SetupAttachment(RootComponent);

	Trigger->SetBoxExtent(FVector(40.0f, 42.0f, 30.0f));
	//StaticMesh'/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1'
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_BOX(TEXT("/Game/InfinityBladeGrassLands/Environments/Breakables/StaticMesh/Box/SM_Env_Breakables_Box1.SM_Env_Breakables_Box1"));
	if (SM_BOX.Succeeded())
	{
		Box->SetStaticMesh(SM_BOX.Object);
	}

	Box->SetRelativeLocation(FVector(0.0f, -3.5f, -30.0f));

	Trigger->SetCollisionProfileName(TEXT("ItemBox"));
	Box->SetCollisionProfileName(TEXT("NoCollision"));

	WeaponItemClass = AMyWeapon::StaticClass();

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	//ParticleSystem'/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh'

	Effect->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> P_CHESTOPEN(TEXT("/Game/InfinityBladeGrassLands/Effects/FX_Treasure/Chest/P_TreasureChest_Open_Mesh.P_TreasureChest_Open_Mesh"));
	if (P_CHESTOPEN.Succeeded())
	{
		Effect->SetTemplate(P_CHESTOPEN.Object);
		Effect->bAutoActivate = false;
	}
}

// Called when the game starts or when spawned
void AMyItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyItemBox::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyItemBox::OnCharacterOverlap);
}

// Called every frame
void AMyItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyItemBox::OnCharacterOverlap(UPrimitiveComponent * OverlappedCom, AActor * OtherActor, UPrimitiveComponent * otherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto myCharacter = Cast<AMyCharacter>(OtherActor);
	if (myCharacter != nullptr)
	{
		if (myCharacter->CanSetWeapon() == true)
		{
			auto NewWeapon = GetWorld()->SpawnActor<AMyWeapon>(WeaponItemClass, FVector::ZeroVector, FRotator::ZeroRotator); //해당클래스로 인스턴스만듦
			myCharacter->SetWeapon(NewWeapon);

			Effect->Activate(true);
			Box->SetHiddenInGame(true, true);
			SetActorEnableCollision(false);
			Effect->OnSystemFinished.AddDynamic(this, &AMyItemBox::OnEffectFinished);
		}
		else
		{
			ALOG(Warning, TEXT("%s Can not equip weapon currently."), *myCharacter->GetName());
		}
	}
}

void AMyItemBox::OnEffectFinished(UParticleSystemComponent * PSystem)
{
	Destroy();
}

