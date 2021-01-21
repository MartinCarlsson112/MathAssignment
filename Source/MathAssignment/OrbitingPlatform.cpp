// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitingPlatform.h"

// Sets default values
AOrbitingPlatform::AOrbitingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOrbitingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrbitingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FRotator CombineRotators(FRotator A, FRotator B)
{
	FQuat AQuat = FQuat(A);
	FQuat BQuat = FQuat(B);

	return FRotator(BQuat * AQuat);
}

TArray<FVector> AOrbitingPlatform::OrbitSolve(TArray<UOrbitingPlatformComponent*> ComponentToSolve)
{
	TArray<FVector> Results;
	int nSolves = ComponentToSolve.Num();
	Results.SetNum(nSolves);
	for (int i = 0; i < nSolves; i++)
	{
		if (ComponentToSolve[i] == nullptr)
		{
			continue;
		}
		float TimeX = GetWorld()->GetTimeSeconds() * ComponentToSolve[i]->SolverData.Rate.X;
		float TimeY = GetWorld()->GetTimeSeconds() * ComponentToSolve[i]->SolverData.Rate.Y;

		FRotator Rot = FRotator(0, (float)((int)(ComponentToSolve[i]->SolverData.Offset.X +
			TimeX) % 360), 0);

		FRotator RotY = FRotator(0, 0 , (float)((int)(ComponentToSolve[i]->SolverData.Offset.Y +
			TimeY) % 360));

		
		FVector A = Rot.RotateVector(FVector(ComponentToSolve[i]->SolverData.Radius.X, 0, 0)), 
		B = RotY.RotateVector(FVector(0, ComponentToSolve[i]->SolverData.Radius.Y, 0));

		Results[i] = A + B;
	}
	return Results;
}


