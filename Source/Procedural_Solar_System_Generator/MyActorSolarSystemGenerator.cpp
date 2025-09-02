// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActorSolarSystemGenerator.h"

// Sets default values
AMyActorSolarSystemGenerator::AMyActorSolarSystemGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyActorSolarSystemGenerator::BeginPlay()
{
	Super::BeginPlay();


    checkf(PlanetsMaterials.Num() > 0, TEXT("It is necessary to define at least one texture for the planets."));

    //Check if we can place the required number of planets
    CheckSolarSystem();
    RandomStream = new FRandomStream();
    RandomStream->Initialize(Seed);
	
    //Create LOD Manager
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    ALODManager* LODManager = GetWorld()->SpawnActor<ALODManager>(
        ALODManager::StaticClass(),
        FVector(0.f,0.f,0.f),
        FRotator::ZeroRotator,
        SpawnParams
    );

    //Generate the Star of the Solar System
    AStar* Star = GenerateStar();
    CelestialBodies.Add(Star);

    //Generate each of the Planets
	for (int i = 0; i < nPlanets; i++) {
		APlanet* Planet = GeneratePlanet();
		if (Planet) {
            CelestialBodies.Add(Planet);
		}
	}

    //Create SpeedManager
    ASpeedManager* SpeedManager = GetWorld()->SpawnActor<ASpeedManager>(
        ASpeedManager::StaticClass(),
        FVector(0.f, 0.f, 0.f),
        FRotator::ZeroRotator,
        SpawnParams
    );


    //Initialize LOD Manager
    LODManager->Initialize(&CelestialBodies, &DistancesLOD);
}

// Called every frame
void AMyActorSolarSystemGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


AStar* AMyActorSolarSystemGenerator::GenerateStar() {

    int32 Radius = RandomStream->RandRange(minimumRadiusStar, maximumRadiusStar);
    float base = RandomStream->FRandRange(minBaseMassStar, maxBaseMassStar);
    float exponent = RandomStream->FRandRange(minExponentMassStar, maxExponentMassStar);
    double Mass = base* pow(10, exponent);

    // Place Star at (0,0,0) Position
    FVector Posicio = FVector(
        0.f,0.f,
        0.0f
    );

    //Spawn Star
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    AStar* StarResult = GetWorld()->SpawnActor<AStar>(
        AStar::StaticClass(),
        Posicio,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (StarResult) {
        UMaterialInstanceDynamic* MaterialEstrellaDynamic = UMaterialInstanceDynamic::Create(StarMaterial, this);
        StarResult->Initialize(Radius,Posicio, Mass, NumSectionsLat, NumSectionsLon,
            NumPointsPerLOD, MaterialEstrellaDynamic,
            StarIntensity, AttenuationRadius);
    }

    return StarResult;
}

void AMyActorSolarSystemGenerator::CheckSolarSystem()
{
    checkf(maxDistanceStar >= minDistanceStar, TEXT("Min distance to star must be smaller than Max distance to star"));
    int space = maxDistanceStar - minDistanceStar;
    int nPossible = int(float(space) / float(minDistPlanets)) + 1;
    checkf(nPossible >= nPlanets, TEXT("Planet distribution not possible"));
}


//Check for each available interval, how many planets fit
//Throw error if not enough space for all the planets that are still not placed
void AMyActorSolarSystemGenerator::CheckPlanets()
{
    int nAvailable = 0;

    for (int i = 0; i < FreeSpacesSolarSystem.Num(); i++) {
        int minDist = FreeSpacesSolarSystem[i].Key;
        int maxDist = FreeSpacesSolarSystem[i].Value;

        int space = maxDist - minDist;
        int nPossible = int(float(space) / float(minDistPlanets)) + 1;

        nAvailable += nPossible;
    }
    int toPlace = nPlanets - CelestialBodies.Num() - 1;
    checkf(nAvailable >= toPlace, TEXT("Planet distribution not possible. Change Seed. nAvailable: %d. toPlace: %d"), nAvailable, toPlace);
}

APlanet* AMyActorSolarSystemGenerator::GeneratePlanet() {

    int32 Radius = RandomStream->RandRange(minPlanetRadius, maxPlanetRadius);
    float base = RandomStream->RandRange(minBaseMassPlanet, maxBaseMassPlanet);
    float exponent = RandomStream->RandRange(minExponentMassPlanet, maxExponentMassStar);
    double Mass = base * pow(10, exponent);

    //Distance Between the center of the Star and center of the planet
    int32 DistStar;

    //Get the distance between Star and Planet
    if (FreeSpacesSolarSystem.Num() == 0) { 
        DistStar = PlaceFirstPlanet();
    }
    else {
        int32 set = SelectUbicationRank();
        DistStar = PlaceOtherPlanets(set);
    }

    CheckPlanets(); //Check if the distribution is achievable

    // Calculate the position of the planet
    float Angle = RandomStream->FRandRange(0.0f, 2 * PI);
    FVector Posicio = FVector(
        DistStar * FMath::Cos(Angle),
        DistStar * FMath::Sin(Angle),
        0.0f
    );

    TArray<float> NoiseScales;
    TArray<int32> Heights;

    float Temperature = CalcTemperature(Posicio);

    UMaterialInstanceDynamic* MaterialFinal = CalculatePlanetMaterial(Temperature);

    for (int i = 0; i < InfoPerlin.Num(); i++) {
        NoiseScales.Emplace(RandomStream->FRandRange(InfoPerlin[i].minNoiseScale, InfoPerlin[i].maxNoiseScale));
        Heights.Emplace(RandomStream->RandRange(InfoPerlin[i].minHeight, InfoPerlin[i].maxHeight));
    }

    //Spawn Planet
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    APlanet* Planet = GetWorld()->SpawnActor<APlanet>(
        APlanet::StaticClass(),
        Posicio,
        FRotator::ZeroRotator,
        SpawnParams
    );

    bool water = false;
    float percHeightWater = 0;
    
    //Check if the planet will have water and its height
    if (Temperature >= minTempWater && Temperature <= maxTempWater) {
        water = RandomStream->FRandRange(0.0f, 1) <= probWater;
        percHeightWater = RandomStream->FRandRange(minPercHeight, maxPercHeight);
    }

    if (Planet) {
        Planet->initialize(Radius, Posicio, Mass, NumSectionsLat, NumSectionsLon, NoiseScales, Heights, NumPointsPerLOD,
            MaterialFinal, DistStar, Angle, move, water, percHeightWater, Temperature, waterMaterial,
            planetsDecorations, RandomStream, Cast<AStar>(CelestialBodies[0]));
            
    }

    return Planet;
}


int AMyActorSolarSystemGenerator::PlaceFirstPlanet()
{
    int DistStar = RandomStream->RandRange(minDistanceStar, maxDistanceStar);

    int minDist = DistStar - minDistPlanets; //Distance to the "Left" of the current Planet
    int maxDist = DistStar + minDistPlanets; //Distance to the "Right" of the curren Planet

    if (minDist >= minDistanceStar && maxDist <= maxDistanceStar) { //Both distances are valid
        int distTotDisponible = (minDist - minDistanceStar) + (maxDistanceStar - maxDist);

        FreeSpacesSolarSystem.Add(FVector2Pair(minDistanceStar, minDist));
        FreeSpacesSolarSystem.Add(FVector2Pair(maxDist, maxDistanceStar));
    }

    else if (minDist < minDistanceStar) { //Can only place more planets to the "Right" of the current planet
        FreeSpacesSolarSystem.Add(FVector2Pair(maxDist, maxDistanceStar));
    }

    else { //Can only place more planets to the "Left" of the current planet
        FreeSpacesSolarSystem.Add(FVector2Pair(minDistanceStar, minDist));
    }

    return DistStar;
}

int AMyActorSolarSystemGenerator::PlaceOtherPlanets(int32 inter)
{
    int32 DistStar = RandomStream->RandRange(FreeSpacesSolarSystem[inter].Key, FreeSpacesSolarSystem[inter].Value); //Distància aleatoria entre el rang seleccionat

    int32 minDist = DistStar - minDistPlanets; //Distance to the "Left" of the current Planet
    int32 maxDist = DistStar + minDistPlanets; //Distance to the "Right" of the curren Planet

    int32 distMinRank = FreeSpacesSolarSystem[inter].Key;
    int32 distMaxRank = FreeSpacesSolarSystem[inter].Value;

    //Both distances are valid. The interval "inter" is no longer valid and must be splitted in 2.
    if (minDist >= distMinRank && maxDist <= distMaxRank) {
        FreeSpacesSolarSystem[inter] = FVector2Pair(distMinRank, minDist);
        FreeSpacesSolarSystem.Add(FVector2Pair(maxDist, distMaxRank));
    }

    else if (minDist < distMinRank && maxDist > distMaxRank) FreeSpacesSolarSystem.RemoveAt(inter);

    else if (minDist < distMinRank) {
        FreeSpacesSolarSystem[inter] = FVector2Pair(maxDist, distMaxRank);
    }

    else  {
        FreeSpacesSolarSystem[inter] = FVector2Pair(distMinRank, minDist);
    }

    return DistStar;
}

int AMyActorSolarSystemGenerator::SelectUbicationRank()
{
    //Currently random between all the available Ranks (intervals)
    return RandomStream->RandRange(0, FreeSpacesSolarSystem.Num() - 1);
}

float AMyActorSolarSystemGenerator::CalcTemperature(FVector Position)
{
    float Distancia = FVector::Distance(Position, Cast<AStar>(CelestialBodies[0])->GetActorLocation());
    
    //Linear interpolation to get the temperature of the planet
    float alfa = (Distancia - minDistanceStar) / (maxDistanceStar - minDistanceStar);

    float temperature = FMath::Lerp(maxTemperature, minTemperature, alfa);

    return temperature; 
}



UMaterialInstanceDynamic* AMyActorSolarSystemGenerator::CalculatePlanetMaterial(float TemPlanet) {
    UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(BaseMaterial, this);

    int32 nText = PlanetsMaterials.Num();

    for (int32 i = 0; i < NMAXTEXTURES; i++) {
        FString TextureName = FString::Printf(TEXT("Texture_%d"), i);
        FString NameTemp = FString::Printf(TEXT("TempOpt_%d"), i);

        if (i < nText) { //We add the user's textures
            DynMat->SetTextureParameterValue(FName(*TextureName), PlanetsMaterials[i].Texture);
            DynMat->SetScalarParameterValue(FName(*NameTemp), PlanetsMaterials[i].OptimalTemperature);
        }

        else { //If user didn't indicate "NMAXTEXTURES", we add the last one to complete the material. 
            DynMat->SetTextureParameterValue(FName(*TextureName), PlanetsMaterials[nText-1].Texture);
            DynMat->SetScalarParameterValue(FName(*NameTemp), std::numeric_limits<int32>::max());
        }

    }
    DynMat->SetScalarParameterValue(FName("PlanetTemperature"), TemPlanet);
    return DynMat;
}

