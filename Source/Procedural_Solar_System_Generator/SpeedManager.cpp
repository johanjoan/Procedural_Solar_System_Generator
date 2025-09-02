// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedManager.h"
#include <GameFramework/FloatingPawnMovement.h>

// Sets default values
ASpeedManager::ASpeedManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}


// Called when the game starts or when spawned
void ASpeedManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASpeedManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (PC->WasInputKeyJustPressed(EKeys::C)) {
			ChangeVelocity();
		}

		else if (PC->WasInputKeyJustPressed(EKeys::J)) {
			IncreaseVelocity();
		}
		else if (PC->WasInputKeyJustPressed(EKeys::K)) {
			DecreaseVelocity();
		}
		else if (PC->WasInputKeyJustPressed(EKeys::R)) {
			RestartVelocity();
		}
	}
}


void ASpeedManager::ChangeVelocity()
{

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		UFloatingPawnMovement* MoveComp = PlayerPawn->FindComponentByClass<UFloatingPawnMovement>();
		if (MoveComp)
		{
			MoveComp->Velocity = FVector::ZeroVector;
			MoveComp->MaxSpeed = 4800;     
			MoveComp->Acceleration = 2048.0f; 
			MoveComp->Deceleration = 9600.0f; 
		}

}

void ASpeedManager::IncreaseVelocity()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UFloatingPawnMovement* MoveComp = PlayerPawn->FindComponentByClass<UFloatingPawnMovement>();

	MoveComp->MaxSpeed = MoveComp->MaxSpeed + MoveComp->MaxSpeed * 0.1;    
	MoveComp->Acceleration = MoveComp->Acceleration + MoveComp->Acceleration * 0.1;
	MoveComp->Deceleration = MoveComp->Deceleration + MoveComp->Deceleration * 0.1;
}

void ASpeedManager::DecreaseVelocity()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UFloatingPawnMovement* MoveComp = PlayerPawn->FindComponentByClass<UFloatingPawnMovement>();

	MoveComp->MaxSpeed = MoveComp->MaxSpeed - MoveComp->MaxSpeed * 0.1;
	MoveComp->Acceleration = MoveComp->Acceleration - MoveComp->Acceleration * 0.1;
	MoveComp->Deceleration = MoveComp->Deceleration - MoveComp->Deceleration * 0.1;
}

void ASpeedManager::RestartVelocity()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	UFloatingPawnMovement* MoveComp = PlayerPawn->FindComponentByClass<UFloatingPawnMovement>();
	if (MoveComp)
	{
		MoveComp->MaxSpeed = 300000.f;     
		MoveComp->Acceleration = 300000.f; 
		MoveComp->Deceleration = 600000.f; 
	}
}



