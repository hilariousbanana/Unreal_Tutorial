// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "GameFramework/Actor.h"
#include "MyItemBox.generated.h"

UCLASS()
class PRACTICE_1027_API AMyItemBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItemBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, Category = "Box")
		UBoxComponent* Trigger;
	UPROPERTY(VisibleAnywhere, Category = "Box")
		UStaticMeshComponent* Box;
	UPROPERTY(EditInstanceOnly, Category = "Box")
		TSubclassOf<class AMyWeapon> WeaponItemClass;
	UPROPERTY(VisibleAnywhere, Category = "Effect")
		UParticleSystemComponent* Effect;

private:
	UFUNCTION()
		void OnCharacterOverlap(UPrimitiveComponent* OverlappedCom, AActor* OtherActor, UPrimitiveComponent* otherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);
	UFUNCTION()
		void OnEffectFinished(class UParticleSystemComponent* PSystem);
};
