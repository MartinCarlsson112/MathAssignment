#pragma once
#include "DrawDebugHelpers.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TurretSolver.generated.h"


UCLASS()
class MATHASSIGNMENT_API UTurretSolver : public UObject
{
	GENERATED_BODY()
	
	UFUNCTION(BlueprintCallable, Category = Turret, meta = (WorldContext = "WorldContextObject"))
	static FVector PredictionSolver(UObject* WorldContextObject, FVector FirePosition, FVector TargetPosition, FVector TargetVelocity, float BulletSpeed, bool DebugDraw = false)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject);
		float Distance = (FirePosition - TargetPosition).Size();
		float Time = Distance / BulletSpeed;
		FVector PredictedPosition = TargetPosition + (TargetVelocity * Time);
		if (DebugDraw)
		{
			DrawDebugSphere(World, PredictedPosition, 200, 26, FColor(181, 0, 0), false, 0.5f, 0, 2);
		}

		return PredictedPosition - FirePosition;
	}


};
