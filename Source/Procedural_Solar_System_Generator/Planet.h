// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialBody.h"
#include "WaterSphere.h"
#include "Components/SpotLightComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Star.h"
#include "Planet.generated.h"


USTRUCT(BlueprintType)
struct FDecoration 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Planets Decorations")
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, Category = "Planets Decorations")
	float OptimalTemperature;

	UPROPERTY(EditAnywhere, Category = "Planets Decorations")
	float Deviation;

	UPROPERTY(EditAnywhere, Category = "Planets Decorations")
	float Probability;

	UPROPERTY(EditAnywhere, Category = "Planets Decorations",meta = (DisplayName = "Scale"))
	FVector Scale = FVector::One();

};


UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  APlanet : public ACelestialBody
{
	GENERATED_BODY()
	
	APlanet();

public:
	void initialize(int32 inRadius, FVector inPosition, double inMass, int32 NumSecLat, int32 NumSecLon, const TArray<float>& inNoiseScale, const TArray<int32>& inHeight,
		const TArray<int>& NumPointsPerLOD, UMaterialInstanceDynamic* inPlanetMaterial, int32 distStar, float angle, bool inMove, bool inWater, float percWaterHeight,
		float inTemperature, UMaterial* inWaterMaterial, TArray<FDecoration>& DecorationArray, FRandomStream* InRandomStream, AStar* InStar);

	virtual bool isPlanet() const override { return true; }
	void CloseToPlayer();
	void FarToPlayer();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:
	int32 OrbitalRadius;
	bool hasWater;
	float temperature;
	AStar* Star;
	FRandomStream* RandomStream;

	void RelocateDecoration();

	AWaterSphere* WaterSphere;
	float calcDecorationProbability(float decorationTemp, float deviation);
	TArray<FDecoration> MyDecorationArray;

	FTimerHandle Timer;
};
