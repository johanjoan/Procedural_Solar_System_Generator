// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Planet.h"
#include "Star.h"
#include "GameFramework/Actor.h"
#include "LODManager.generated.h"

UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  ALODManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALODManager();
	void Initialize(TArray<ACelestialBody*>* InPlanets, TArray<int>* InDistancesLOD);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	TArray<ACelestialBody*>* CelestialBodies;  
	TArray<int>* DistancesLOD;  

	void updateLODs();
	void UpdateLODNearPlanet();

	APlanet* PlanetLOD = nullptr; //Reference to the planet with higher LOD
};
