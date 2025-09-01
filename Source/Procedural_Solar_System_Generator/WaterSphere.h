// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereSection.h"
#include "WaterSphere.generated.h"

UCLASS()
class PROCEDURAL_SOLAR_SYSTEM_GENERATOR_API  AWaterSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaterSphere();

	void Initialize(int InRadius, int NumSecLat, int NumSecLon, const TArray<int>& NumPointsPerLOD, FVector InPosition, UMaterial* WaterMaterial);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	TArray<ASphereSection*> MySphereSections;
	int32 Radius;
	UMaterialInstanceDynamic* DynamicWaterMaterial;
};
