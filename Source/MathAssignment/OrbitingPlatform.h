// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbitingPlatformComponent.h"
#include "OrbitingPlatform.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MATHASSIGNMENT_API AOrbitingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOrbitingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Orbit)
	TArray<FVector> OrbitSolve(TArray<UOrbitingPlatformComponent*> ComponentToSolve);

};
