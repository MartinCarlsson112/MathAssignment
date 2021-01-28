#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OrbitComponent.h"
#include "OrbitSolve.generated.h"


static FRotator CombineRotators(FRotator A, FRotator B)
{
	FQuat AQuat = FQuat(A);
	FQuat BQuat = FQuat(B);

	return FRotator(BQuat * AQuat);
}

UCLASS()
class MATHASSIGNMENT_API UOrbitSolver : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = Orbit)
	static void RotationSolve(TArray<UOrbitComponent*> ComponentToSolve, FVector Speed, float Time)
	{
		int nSolves = ComponentToSolve.Num();
		for (int i = 0; i < nSolves; i++)
		{
			if (ComponentToSolve[i] == nullptr)
			{
				continue;
			}
			FRotator OldRotation = ComponentToSolve[i]->GetRelativeRotation();
			ComponentToSolve[i]->SetRelativeRotation(FRotator(OldRotation.Vector().X + Speed.X * Time, OldRotation.Vector().Y + Speed.Y * Time, OldRotation.Vector().Z + Speed.Z * Time ));
		}
	}
	
	UFUNCTION(BlueprintCallable, Category = Orbit)
	static FVector SolveOrbit(FOrbitSolverData Data, FVector OrbitPivot, float Time)
	{
		FVector Rate = Data.Rate;
		FVector Offset = Data.Offset;
		FVector Radius = Data.Radius;
		float TimeX = Time * Rate.X;
		float TimeY = Time * Rate.Y;
		float TimeZ = Time * Rate.Z;

		FRotator Rot = FRotator(0, FMath::Fmod(Offset.X + TimeX, 360), 0);

		FRotator RotY = FRotator(0, 0, FMath::Fmod(Offset.Y + TimeY, 360));

		FRotator RotZ = FRotator(FMath::Fmod(Offset.Z + TimeZ, 360), 0, 0);

		FVector A = Rot.RotateVector(FVector(Radius.X, 0, 0)),
			B = RotY.RotateVector(FVector(0, Radius.Y, 0)),
			C = RotZ.RotateVector(FVector(0, 0, Radius.Z));

		return OrbitPivot + A + B + C;
	}

	UFUNCTION(BlueprintCallable, Category = Orbit)
	static void OrbitSolveMultipleComponents(TArray<UOrbitComponent*> ComponentToSolve, FVector OrbitPivot, float Time)
	{
		int nSolves = ComponentToSolve.Num();
		for (int i = 0; i < nSolves; i++)
		{
			OrbitSolveComponent(ComponentToSolve[i], OrbitPivot, Time);
		}
	}



	UFUNCTION(BlueprintCallable, Category = Orbit)
		static void OrbitSolveComponent(UOrbitComponent* ComponentToSolve, FVector OrbitPivot, float Time)
	{
		if (ComponentToSolve == nullptr)
		{
			return;
		}
		FVector NewPos = SolveOrbit(ComponentToSolve->SolverData, OrbitPivot, Time);
		ComponentToSolve->SetRelativeLocation(NewPos, false, nullptr, ETeleportType::TeleportPhysics);
	}
};
