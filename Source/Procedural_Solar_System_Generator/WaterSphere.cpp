// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterSphere.h"

// Sets default values
AWaterSphere::AWaterSphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Default scene component"));
    SetRootComponent(root);
}

void AWaterSphere::Initialize(int InRadius, int NumSecLat, int NumSecLon, const TArray<int>& NumPointsPerLOD, FVector InPosition, UMaterial* WaterMaterial)
{
	Radius = InRadius;
    PerlinNoise* Noise = new PerlinNoise();

    //Crate Sections of water sphere
    for (int i = 0; i < NumSecLat; i++) {
        for (int j = 0; j < NumSecLon; j++) {
            UChildActorComponent* NewChildComponent = NewObject<UChildActorComponent>(this);
            NewChildComponent->RegisterComponent();
            NewChildComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
            NewChildComponent->SetChildActorClass(ASphereSection::StaticClass());
        }
    }

    TArray<int32> Heights;
    Heights.Emplace(0);
    TArray<float> NoiseScales;
    NoiseScales.Emplace(0);

    //Create Sections meshes
    TArray<UChildActorComponent*> SphereSections;
    GetComponents<UChildActorComponent>(SphereSections);
    DynamicWaterMaterial = UMaterialInstanceDynamic::Create(WaterMaterial, this);
    float latStep = 180.f / NumSecLat;
    float lonStep = 360.f / NumSecLon;

    //We just add a mesh for Water
    int j = 0; 
    for (UChildActorComponent* Section : SphereSections) {
        ASphereSection* SphereSection = Cast<ASphereSection>(Section->GetChildActor());
        if (SphereSection) {
            MySphereSections.Add(SphereSection); 
            int latIndex = j / NumSecLat;
            int lonIndex = j % NumSecLon;
            float minLat = -90.f + latIndex * latStep;
            float maxLat = minLat + latStep;
            float minLon = -180.f + lonIndex * lonStep;
            float maxLon = minLon + lonStep;
            SphereSection->Initialize(Radius, NoiseScales, Heights, Noise, minLat, maxLat, minLon, maxLon, DynamicWaterMaterial);
            j++;
            
            SphereSection->CreateNewMesh(NumPointsPerLOD[1]);
        }
    }
}

// Called when the game starts or when spawned
void AWaterSphere::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWaterSphere::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

