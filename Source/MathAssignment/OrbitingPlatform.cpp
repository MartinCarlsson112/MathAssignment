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

FVector AOrbitingPlatform::OrbitSolve(float Radius, float Offset, float Rate)
{
	float Time = GetWorld()->GetTimeSeconds() * Rate;

	FRotator Rot = FRotator(0, (float)((int)(Offset +
		Time) % 360), 0);

	FVector RotatedVector = Rot.RotateVector(FVector(Radius, 0, 0));


	return	RotatedVector;
}

