// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CelestialBody.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "SpeedManager.generated.h"

UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  ASpeedManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpeedManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ChangeVelocity();
	void IncreaseVelocity();
	void DecreaseVelocity();
	void RestartVelocity();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
