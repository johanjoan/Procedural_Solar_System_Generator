// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <random>
#include "LODManager.h"
#include "SpeedManager.h"
#include "CelestialBody.h"
#include "Planet.h"
#include "Star.h"
#include "MyActorSolarSystemGenerator.generated.h"

#define NMAXTEXTURES 8

USTRUCT(BlueprintType)
struct FPairTextures
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Planets Properties")
	UTexture2D* Texture;

	UPROPERTY(EditAnywhere, Category = "Planets Properties")
	float OptimalTemperature;
};

USTRUCT(BlueprintType)
struct FInfoPerlin
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Minimum Perlin Frequency Value"))
	float minNoiseScale = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Maximum Perlin Frequency Value"))
	float maxNoiseScale = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Minimum Height Value"))
	int minHeight = 1500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Maximum Height Value"))
	int maxHeight = 2000;
};


UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  AMyActorSolarSystemGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActorSolarSystemGenerator();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Solar System Properties", meta = (DisplayName = "Random seed value"))
	int Seed = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Solar System Properties", meta = (DisplayName = "Number of Planets"))
	int nPlanets = 8;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties", meta = (DisplayName = "Minimum Radius"))
	int minimumRadiusStar= 200000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties", meta = (DisplayName = "Maximum Radius"))
	int maximumRadiusStar = 200000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties",
		meta = (DisplayName = "Minimum value of Star Mass Base"))
	float minBaseMassStar = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties",
		meta = (DisplayName = "Maximum value of Star Mass Base"))
	float maxBaseMassStar = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties",
		meta = (DisplayName = "Minimum value of Star Mass Exponent"))
	float minExponentMassStar = 23;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties",
		meta = (DisplayName = "Maximum value of Star Mass Exponent"))
	float maxExponentMassStar = 23;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties", meta = (DisplayName = "Star's Light Intensity"))
	float StarIntensity = 10000000000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties", meta = (DisplayName = "Star's Light Attenuation Radius"))
	float AttenuationRadius = 10000000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Star Properties", meta = (DisplayName = "Star Material"))
	UMaterial* StarMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Minumum Planet Radius"))
	int minPlanetRadius = 100000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Maximum Planet Radius"))
	int maxPlanetRadius = 150000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Minimum Distance to Star"))
	int minDistanceStar = 800000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Maximum Distance to Star"))
	int maxDistanceStar = 9050000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Minimum Distance Betweetn Planets"))
	int minDistPlanets = 700000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Minimum Planet Temperature"))
	float minTemperature = 0.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Maximum Planet Temperature"))
	float maxTemperature = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", meta = (DisplayName = "Planets Move?"))
	bool move = false;

	UPROPERTY(meta = (ClampMin = 0, ClampMax = 1), EditAnywhere, BlueprintReadWrite, Category = "Planets Properties", 
		meta = (DisplayName = "Water Presence Probability"))
	float probWater = 1; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Minimum value of Planet Mass Base"))
	float minBaseMassPlanet = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Maximum value of Star Mass Base"))
	float maxBaseMassPlanet = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Minimum value of Planet Mass Exponent"))
	float minExponentMassPlanet = 22;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planets Properties",
		meta = (DisplayName = "Maximum value of Planet Mass Exponent"))
	float maxExponentMassPlanet = 22;

	UPROPERTY(EditAnywhere, Category = "Planets Properties", meta = (DisplayName = "Perlin Noise Values"))
	TArray<FInfoPerlin> InfoPerlin = {
		FInfoPerlin{0.5f, 1.5f,60000,80000},
		FInfoPerlin{3.0f, 6.0f,50000,60000},
		FInfoPerlin{8.0f, 10.0f,30000,50000}
	};
	
	UPROPERTY(EditAnywhere, Category = "Planets Properties", meta = (DisplayName = "Planets Textures"))
	TArray<FPairTextures> PlanetsMaterials;

	UPROPERTY(EditAnywhere, Category = "Planets Decorations", meta = (DisplayName = "Planets Decorations Information"))
	TArray<FDecoration> planetsDecorations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Properties", meta = (DisplayName = "Water Material"))
	UMaterial* waterMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Properties", 
		meta = (DisplayName = "Minimum Temperature of the Planet that can have Water"))
	float minTempWater = 25;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Properties",
		meta = (DisplayName = "Maximum Temperature of the Planet that can have Water"))
	float maxTempWater = 60;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Properties",
		meta = (DisplayName = "Minimum Percentage of Water Heigh"))
	float minPercHeight = 0.85; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Water Properties",
		meta = (DisplayName = "Maximum Percentage of Water Heigh"))
	float maxPercHeight = 0.9;

	UPROPERTY(EditAnywhere, Category = "Level Of Detail", meta = (DisplayName = "Latitude Number of Divisons"))
	int NumSectionsLat = 6;

	UPROPERTY(EditAnywhere, Category = "Level Of Detail", meta = (DisplayName = "Longitude Number of Divisons"))
	int NumSectionsLon = 6;

	UPROPERTY(EditAnywhere, Category = "Level Of Detail", meta = (DisplayName = "Number of Vertices per Longitude and Latitude"))
	TArray<int> NumPointsPerLOD = {3, 6, 100}; 

	UPROPERTY(EditAnywhere, Category = "Level Of Detail", meta = (DisplayName = "Distances for Each Level of Detail"))
	TArray<int> DistancesLOD = { 800000, 250000, 0 };

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	TArray<ACelestialBody*> CelestialBodies;

	APlanet* GeneratePlanet();
	AStar* GenerateStar();

	FRandomStream* RandomStream; 

	void CheckSolarSystem();
	void CheckPlanets();
	int PlaceFirstPlanet();
	int PlaceOtherPlanets(int32 set);
	int SelectUbicationRank();

	using FVector2Pair = TPair<int32, int32>; //<minPos,maxPos>
		
	TArray<FVector2Pair> FreeSpacesSolarSystem; //For each available interval of the solar system has its minPos and maxPos where a planet can be placed

	float CalcTemperature(FVector Position);

	UMaterialInstanceDynamic* CalculatePlanetMaterial(float TemPlanet);
	UPROPERTY(EditAnywhere, Category = "Planets Properties")
	UMaterialInterface* BaseMaterial;
};
