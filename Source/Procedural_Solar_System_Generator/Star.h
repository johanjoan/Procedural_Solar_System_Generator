// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "Components/PointLightComponent.h"
#include "Star.generated.h"


UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  AStar : public ACelestialBody
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AStar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Initialize(int32 inRadius, FVector inPosition, double inMass, int32 NumSecLat, int32 NumSecLon,
		const TArray<int>& NumPointsPerLOD, UMaterialInstanceDynamic* InStarMaterial, float InIntensity, float InAttenuationRadius);


private:
	double Intensity;
	double AttenuationRadius;
	UPointLightComponent* PointLight;
};

