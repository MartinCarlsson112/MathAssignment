#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitSolverData.h"
#include "OrbitingPlatformComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MATHASSIGNMENT_API UOrbitingPlatformComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	UOrbitingPlatformComponent();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Orbit)
	FOrbitSolverData SolverData;

};
