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
	AOrbitingPlatform();

	UFUNCTION(BlueprintCallable, Category = Orbit)
	void OrbitSolveMultiple(TArray<UOrbitingPlatformComponent*> ComponentToSolve, FVector OrbitPivot = FVector(0,0,0), bool bUseVInterp = true, float VInterpSpeed = 1);


	UFUNCTION(BlueprintCallable, Category = Orbit)
	void OrbitSolve(UOrbitingPlatformComponent* ComponentToSolve, FVector OrbitPivot = FVector(0, 0, 0), bool bUseVInterp = true, float VInterpSpeed = 1);


};
