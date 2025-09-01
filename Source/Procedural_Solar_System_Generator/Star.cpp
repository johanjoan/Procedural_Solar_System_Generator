// Fill out your copyright notice in the Description page of Project Settings.


#include "Star.h"

AStar::AStar()
{
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	SetRootComponent(Root);
	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(RootComponent);
}

void AStar::BeginPlay()
{
	Super::BeginPlay();
}

void AStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AStar::Initialize(int32 inRadius, FVector inPosition, double inMass, int32 NumSecLat, int32 NumSecLon,
	const TArray<int>& NumPointsPerLOD, UMaterialInstanceDynamic* InStarMaterial, float InIntensity, float InAttenuationRadius)
{
	TArray<int32> Heights;
	Heights.Emplace(0);
	TArray<float> NoiseScales;
	NoiseScales.Emplace(0);

	Super::initialize(inRadius, inPosition, inMass, NumSecLat, NumSecLon, NoiseScales, Heights,
		NumPointsPerLOD, InStarMaterial);

	Intensity = InIntensity;
	AttenuationRadius = InAttenuationRadius;

	PointLight->SetIntensity(Intensity);
	PointLight->SetLightColor(FLinearColor::White);
	PointLight->SetAttenuationRadius(AttenuationRadius);
	PointLight->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

}
