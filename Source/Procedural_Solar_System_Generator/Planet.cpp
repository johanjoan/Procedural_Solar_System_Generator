// Fill out your copyright notice in the Description page of Project Settings.


#include "Planet.h"


APlanet::APlanet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts or when spawned
void APlanet::BeginPlay()
{
	Super::BeginPlay();
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (isMoving) {
		double GravitationalConstant = 6.674e-7; // G in cm³/(kg·s²)

		FVector CurrentLocation = GetActorLocation();

		FVector StarPosition = Star->GetActorLocation();

		FVector PlanetPosition = GetActorLocation();

		FVector Direction = (StarPosition - PlanetPosition).GetSafeNormal();
		float Distance = FVector::Distance(StarPosition, PlanetPosition);

		// Gravitational Acceleration (in cm/s²)
		float Acceleration = GravitationalConstant * Star->getMass() / (Distance * Distance);
		CurrentVelocity += Direction * Acceleration * DeltaTime;

		// Update Position
		SetActorLocation(PlanetPosition + CurrentVelocity * DeltaTime);

	};
}


void APlanet::initialize(int32 inRadius, FVector inPosition, double inMass, int32 NumSecLat, int32 NumSecLon, const TArray<float>& inNoiseScale, const TArray<int32>& inHeight,
	const TArray<int>& NumPointsPerLOD, UMaterialInstanceDynamic* inPlanetMaterial, int32 distStar, float angle, bool inMove, bool inWater, float percWaterHeight,
	float inTemperature, UMaterial* inWaterMaterial, TArray<FDecoration>& DecorationArray, FRandomStream* InRandomStream, AStar* InStar)
{
	Super::initialize(inRadius, inPosition, inMass, NumSecLat, NumSecLon, inNoiseScale, inHeight,
		NumPointsPerLOD, inPlanetMaterial);

	Star = InStar;
	RandomStream = InRandomStream;
	OrbitalRadius = distStar;
	isMoving = inMove;
	isMovingGlobal = inMove;
	hasWater = inWater;
	temperature = inTemperature;
	MyDecorationArray = DecorationArray;


	if (hasWater) {
		int radiAigua = MaxHeight * percWaterHeight;
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		WaterSphere = GetWorld()->SpawnActor<AWaterSphere>(
			AWaterSphere::StaticClass(),
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		

		if (WaterSphere) {
			WaterSphere->Initialize(radiAigua, NumSecLat, NumSecLon, NumPointsPerLOD, inPosition, inWaterMaterial);
			WaterSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}

	// Calc Orbital Velocity
	double GravitationalConstant = 6.674e-7; // G en cm³/(kg·s²)
	float Speed = FMath::Sqrt(GravitationalConstant * Star->getMass() / OrbitalRadius);

	FVector VelocitatDirection = FVector(-FMath::Sin(angle), FMath::Cos(angle), 0.0f);
	CurrentVelocity = VelocitatDirection * Speed;
	
	float stepLat, stepLon;
	for (auto& decoration : DecorationArray) { //Iterate over all the decorations
		// Calc probability that the decoration is in the planet depending on the temperature
		float probTemperature = calcDecorationProbability(decoration.OptimalTemperature, decoration.Deviation);
		if (probTemperature > 0) {
			for (auto& section : MySphereSections) {
				// Create a new instance of the decoration to the Planet secction
				section->AddDecoration(decoration.Mesh);
				for (float lat = section->getMinLat(); lat < section->getMaxLat(); lat+= stepLat) {
					for (float lon = section->getMinLon(); lon < section->getMaxLon(); lon+= stepLon) {
						float prob = RandomStream->FRandRange(0, 1);
						if (prob <= decoration.Probability* probTemperature) {
							section->addActor(lat,lon, decoration.Mesh, decoration.Scale); 
						}
						stepLon = RandomStream->FRandRange(0, 2);
					}
					stepLat = RandomStream->FRandRange(0, 2);
				}
			}
		}
	}
}

void APlanet::CloseToPlayer()
{
	isMoving = false;
	GetWorld()->GetTimerManager().ClearTimer(Timer); //Cencel Timer
}

void APlanet::FarToPlayer()
{
	GetWorld()->GetTimerManager().SetTimer(
		Timer,
		this,                             
		&APlanet::RelocateDecoration,
		10.0f,                             
		false                             
	);
	isMoving = isMovingGlobal;
}



void APlanet::RelocateDecoration()
{
	for (auto& seccio : MySphereSections) {
		seccio->CleanDecorations(); //Remove all the current decorations
		float stepLat, stepLon;

		for (auto& decoration: MyDecorationArray) {
			float probTemperature = calcDecorationProbability(decoration.OptimalTemperature, decoration.Deviation);
			seccio->AddDecoration(decoration.Mesh);
			for (float lat = seccio->getMinLat(); lat < seccio->getMaxLat(); lat += stepLat) {
				for (float lon = seccio->getMinLon(); lon < seccio->getMaxLon(); lon += stepLon) {
					float prob = RandomStream->FRandRange(0, 1);
					if (prob <= decoration.Probability * probTemperature) {
						seccio->addActor(lat, lon, decoration.Mesh, decoration.Scale);
					}
					stepLon = RandomStream->FRandRange(0, 2);
				}
				stepLat = RandomStream->FRandRange(0, 2); 
			}
		}
	}
}

//Probability according to a Gaussian Distribution
float APlanet::calcDecorationProbability(float decorationTemp, float deviation)
{
	float dif = temperature - decorationTemp;
	return FMath::Exp(-(dif * dif) / (2.0f * deviation * deviation));
}




