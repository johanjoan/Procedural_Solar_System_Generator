// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereSection.h"
#include "CelestialBody.generated.h"

UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  ACelestialBody : public AActor
{
	GENERATED_BODY()
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Sets default values for this actor's properties
	ACelestialBody();

	int32 getRadius();
	double getMass();
	void initialize(int32 inRadius, FVector inPosition, double inMass, int32 NumSecLat, int32 NumSecLon, const TArray<float>& inNoiseScale, const TArray<int32>& inHeight,
		const TArray<int>& NumPointsPerLOD, UMaterialInstanceDynamic* CelestialBodyMaterial);

	const TArray<ASphereSection*>& getSections();

	virtual bool isPlanet() const { return false; }

	float getMaxHeight();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector CurrentVelocity;
	int32 Radius;
	double Mass;
	bool isMoving;
	float MaxHeight;
	bool isMovingGlobal;

	PerlinNoise* Noise;
	TArray<ASphereSection*> MySphereSections;
};
