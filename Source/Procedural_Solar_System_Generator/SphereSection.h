// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "PerlinNoise.h"
#include "Components/StaticMeshComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Camera/PlayerCameraManager.h"
#include "SphereSection.generated.h"

UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  ASphereSection : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASphereSection();

	virtual void Tick(float DeltaTime) override;

	virtual void CreateNewMesh(int n); 

	void Initialize(int inRadius, const TArray<float>& inNoiseScale, const TArray<int32>& inHeight,
		PerlinNoise* InNoise, float InminLat, float InmaxLat, float InminLon, float InmaxLon, UMaterialInstanceDynamic* InMaterial);
	
	void UpdateLOD(int InLOD);
	void ActivateDecorationCollisions();
	void DeactivateDecorationCollisions();


	float getMinLat();
	float getMinLon();
	float getMaxLat();
	float getMaxLon();

	bool isSectioninViewPort();
	void addActor(float lat, float lon, UStaticMesh* Mesh, FVector Scale); //TODO: POTSER CANVIAR EL NOM A ADDDECORACIO O ALGO AIXI

	void AddDecoration(UStaticMesh* Mesh);
	float getMaxHeight();
	void CleanDecorations();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, Category = "Planet")
	UProceduralMeshComponent* MyMesh;

private:
	FVector CalculateNormal(const FVector& point, float lat, float lon);
	FVector CalculatePointFromLatitudeLongitude(float latitude, float longitude);

	FVector CalculateMeshPoint(float Lat, float Lon);


	float minLat, maxLat, minLon, maxLon;
	int32 radius;
	float noiseScale;
	int LOD;

	UPROPERTY()
	TArray<UProceduralMeshComponent*> MyMeshes;
	
	PerlinNoise* Noise;

	UMaterialInterface* CustomMaterial;

	TArray<FTransform> SavedRelativeTransforms;

	TArray<float> NoiseScales;
	TArray<int32> Heights;
	TArray<UHierarchicalInstancedStaticMeshComponent*> ArrayInstancedDecoracions;

	float MaxHeight = 0;
};
