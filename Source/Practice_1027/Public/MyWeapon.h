// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Practice_1027.h"
#include "GameFramework/Actor.h"
#include "MyWeapon.generated.h"

UCLASS()
class PRACTICE_1027_API AMyWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
		USkeletalMeshComponent* Weapon;
};
