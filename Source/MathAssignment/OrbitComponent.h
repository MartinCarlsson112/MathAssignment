#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitSolverData.h"
#include "OrbitComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MATHASSIGNMENT_API UOrbitComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	UOrbitComponent() {
		PrimaryComponentTick.bCanEverTick = false;
	}


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Orbit)
	FOrbitSolverData SolverData;

};
