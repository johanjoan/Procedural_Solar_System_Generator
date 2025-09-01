// Fill out your copyright notice in the Description page of Project Settings.


#include "LODManager.h"

// Sets default values
ALODManager::ALODManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALODManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ALODManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	updateLODs();
}


void ALODManager::updateLODs() {
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();


	for (int i = 0; i < CelestialBodies->Num(); i++) {
		if (PlanetLOD && PlanetLOD == (*CelestialBodies)[i]) {
			UpdateLODNearPlanet();
			continue;
		}

		const TArray<ASphereSection*>& sections = (*CelestialBodies)[i]->getSections();
		for (auto& section : sections) {
			int nDist = DistancesLOD->Num();
			float Distance = FVector::Dist(PlayerLocation, section->GetActorLocation()) - (*CelestialBodies)[i]->getRadius();
			bool updated = false;
			for (int j = 0; j < (nDist-1) && !updated; j++) {
				if (Distance > (*DistancesLOD)[j]) {
					section->UpdateLOD(j);
					updated = true;
				}
			}
			if (!updated) { //Case with more resolution
				if (!(*CelestialBodies)[i]->isPlanet()) section->UpdateLOD(nDist - 1); //Star always with highest LOD

				else {
					if (PlanetLOD == nullptr) { //First planet that needs to have highest LOD
						PlanetLOD = Cast<APlanet>((*CelestialBodies)[i]);
						section->UpdateLOD(nDist - 1);
						PlanetLOD->CloseToPlayer();
					}
					else {
						section->UpdateLOD(nDist - 1); //Second highest LOD
					}
				}
			}
		}
	}

}


void ALODManager::Initialize(TArray<ACelestialBody*>* InPlanets, TArray<int>* InDistancesLOD)
{
	CelestialBodies = InPlanets;
	DistancesLOD = InDistancesLOD;
}

//LOD per section based on distances to player (Latitude and longitude)
void ALODManager::UpdateLODNearPlanet() {

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
	FVector PlanetLocation = PlanetLOD->GetActorLocation();

	float DistanceToPlanet = FVector::Dist(PlayerLocation, PlanetLocation) - PlanetLOD->getRadius();

	if (DistanceToPlanet > (*DistancesLOD)[DistancesLOD->Num() - 2]) { //Check if player moved far away from planet
		PlanetLOD->FarToPlayer();
		PlanetLOD = nullptr;
		return;
	}

	const TArray<ASphereSection*>& sections = PlanetLOD->getSections();
	for (auto& section : sections) {
		int nDist = DistancesLOD->Num();

		// Calc latitude and longitude of player with respect to planet
		FVector SectionLocation = section->GetActorLocation();
		FVector V = PlayerLocation - SectionLocation;
		FVector D = V.GetSafeNormal();

		float playerLat = FMath::RadiansToDegrees(asin(D.Z));
		float playerLon = FMath::RadiansToDegrees(atan2(D.Y, D.X));

		// Ranges of the section 
		float minLat = section->getMinLat();
		float maxLat = section->getMaxLat();
		float minLon = section->getMinLon();
		float maxLon = section->getMaxLon();

		// Check if player is "inside" the section
		bool insideLatitude = (playerLat >= minLat && playerLat <= maxLat);
		bool insideLongitude = (playerLon >= minLon && playerLon <= maxLon);
		bool isSectionPlayer = insideLatitude && insideLongitude;

		if (isSectionPlayer) {
			section->UpdateLOD(nDist - 1);
			section->ActivateDecorationCollisions();
			continue;
		}

		// Check with certain margin
		float marginLat = 60.0f;
		float marginLon = 60.0f;


		if (playerLat >= 35 || playerLat <= -35) marginLon = 100.0f;

		float adjustedMinLat = FMath::Max(minLat - marginLat, -90.0f);
		float adjustedMaxLat = FMath::Min(maxLat + marginLat, 90.0f);
		insideLatitude = (playerLat >= adjustedMinLat && playerLat <= adjustedMaxLat);

		// Normalize section longitudes
		float adjustedMinLon = FMath::Fmod(minLon - marginLon + 180.0f, 360.0f);
		if (adjustedMinLon < 0.0f) adjustedMinLon += 360.0f;
		adjustedMinLon -= 180.0f;

		float adjustedMaxLon = FMath::Fmod(maxLon + marginLon + 180.0f, 360.0f);
		if (adjustedMaxLon < 0.0f) adjustedMaxLon += 360.0f;
		adjustedMaxLon -= 180.0f;

		// Check if normalized longitud is inside the section range
		if (adjustedMinLon <= adjustedMaxLon) {
			insideLongitude = (playerLon >= adjustedMinLon && playerLon <= adjustedMaxLon);
		}
		else {
			// Range crosses ±180° limit
			insideLongitude = (playerLon >= adjustedMinLon || playerLon <= adjustedMaxLon);
		}

		bool insideSection = (insideLatitude && insideLongitude);

		bool esAdalt = (playerLat >= 45 && (maxLat >=60) || (playerLat <= -45 && (minLat <= -60)));

		if ((insideSection || esAdalt) && section->isSectioninViewPort()) {
			section->UpdateLOD(nDist - 1);
			section->ActivateDecorationCollisions();
		}
		else {
			section->UpdateLOD(0);
		}
	}

}
