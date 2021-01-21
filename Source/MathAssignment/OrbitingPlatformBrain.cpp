// Fill out your copyright notice in the Description page of Project Settings.


#include "OrbitingPlatformBrain.h"

// Sets default values
AOrbitingPlatformBrain::AOrbitingPlatformBrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AOrbitingPlatformBrain::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AOrbitingPlatformBrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

