// Fill out your copyright notice in the Description page of Project Settings.


#include "CelestialBody.h"

// Sets default values
ACelestialBody::ACelestialBody()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Default scene component"));
	SetRootComponent(root);
}

int ACelestialBody::getRadius()
{
	return Radius;
}

double ACelestialBody::getMass()
{
	return Mass;
}

void ACelestialBody::initialize(int32 inRadius, FVector inPosition, double inMass, int32 NumSecLat, int32 NumSecLon, const TArray<float>& inNoiseScale, const TArray<int32>& inHeight,
    const TArray<int>& NumPointsPerLOD, UMaterialInstanceDynamic* CelestialBodyMaterial)
{
    MaxHeight = 0;
	Radius = inRadius;
	Mass = inMass;
    isMoving = false;
    CurrentVelocity = FVector(0.0, 0.0, 0.0);
	SetActorLocation(inPosition);

    //Initialize Perlin Noise
    Noise = new PerlinNoise();

	//Create Celestial Body sections
	for (int i = 0; i < NumSecLat; i++) {
		for (int j = 0; j < NumSecLon; j++) {
			UChildActorComponent* NewChildComponent = NewObject<UChildActorComponent>(this); 
			NewChildComponent->RegisterComponent();
			NewChildComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			NewChildComponent->SetChildActorClass(ASphereSection::StaticClass());
		}
	}

    // For each childcomponent (section) we create the meshes For each LOD
    TArray<UChildActorComponent*> SphereSections;
    GetComponents<UChildActorComponent>(SphereSections);
    float latStep = 180.f / NumSecLat;
    float lonStep = 360.f / NumSecLon;
    for (int i = 0; i < NumPointsPerLOD.Num(); i++) { 
        int j = 0; //To know the latitude and longitude ranges 

        for (UChildActorComponent* Section : SphereSections) {
            ASphereSection* SphereSection = Cast<ASphereSection>(Section->GetChildActor());
            if (SphereSection) {
                if (i == 0) { //In the first loop we initialize the section.
                    MySphereSections.Add(SphereSection); //Add each section to the array
                    int latIndex = j / NumSecLat;
                    int lonIndex = j % NumSecLon;
                    float minLat = -90.f + latIndex * latStep;
                    float maxLat = minLat + latStep;
                    float minLon = -180.f + lonIndex * lonStep;
                    float maxLon = minLon + lonStep;
                    SphereSection->Initialize(Radius, inNoiseScale, inHeight, Noise, minLat, maxLat, minLon, maxLon, CelestialBodyMaterial);
                    j++;
                }
                //Create a new mesh for each section and LOD
                SphereSection->CreateNewMesh(NumPointsPerLOD[i]);
                //Check the maximum height of the Celesetial Body
                float height = SphereSection->getMaxHeight();
                if (height > MaxHeight) MaxHeight = height;
            }
        }
    }
}

float ACelestialBody::getMaxHeight()
{
    return MaxHeight;
}


// Called when the game starts or when spawned
void ACelestialBody::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void ACelestialBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


const TArray<ASphereSection*>& ACelestialBody::getSections()
{
    return MySphereSections;
}
