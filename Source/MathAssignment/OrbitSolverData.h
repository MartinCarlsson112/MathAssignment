#pragma once
#include "CoreMinimal.h"
#include "OrbitSolverData.generated.h"

USTRUCT(BlueprintType)
struct FOrbitSolverData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OrbitSolveData)
	FVector Radius;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OrbitSolveData)
	FVector Offset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = OrbitSolveData)
	FVector Rate;
};
