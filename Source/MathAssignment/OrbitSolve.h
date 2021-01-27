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
	static void OrbitSolveMultiple(TArray<UOrbitComponent*> ComponentToSolve, FVector OrbitPivot, float time)
	{
		int nSolves = ComponentToSolve.Num();
		for (int i = 0; i < nSolves; i++)
		{
			if (ComponentToSolve[i] == nullptr)
			{
				continue;
			}
			float TimeInSeconds = time;


			FVector Rate = ComponentToSolve[i]->SolverData.Rate;

			float TimeX = TimeInSeconds * Rate.X;
			float TimeY = TimeInSeconds * Rate.Y;
			float TimeZ = TimeInSeconds * Rate.Z;

			FVector Offset = ComponentToSolve[i]->SolverData.Offset;
			FVector Radius = ComponentToSolve[i]->SolverData.Radius;

			FRotator Rot = FRotator(0, FMath::Fmod(Offset.X + TimeX, 360), 0);

			FRotator RotY = FRotator(0, 0, FMath::Fmod(Offset.Y + TimeY, 360));

			FRotator RotZ = FRotator(FMath::Fmod(Offset.Z + TimeZ, 360), 0, 0);

			FVector A = Rot.RotateVector(FVector(Radius.X, 0, 0)),
				B = RotY.RotateVector(FVector(0, Radius.Y, 0)),
				C = RotZ.RotateVector(FVector(0, 0, Radius.Z));

			ComponentToSolve[i]->SetRelativeLocation(OrbitPivot + A + B + C, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}



	UFUNCTION(BlueprintCallable, Category = Orbit)
		static void OrbitSolve(UOrbitComponent* ComponentToSolve, FVector OrbitPivot, float time)
	{
		if (ComponentToSolve == nullptr)
		{
			return;
		}
		float TimeInSeconds = time;


		FVector Rate = ComponentToSolve->SolverData.Rate;

		float TimeX = TimeInSeconds * Rate.X;
		float TimeY = TimeInSeconds * Rate.Y;
		float TimeZ = TimeInSeconds * Rate.Z;

		FVector Offset = ComponentToSolve->SolverData.Offset;
		FVector Radius = ComponentToSolve->SolverData.Radius;

		FRotator Rot = FRotator(0, FMath::Fmod(Offset.X + TimeX, 360), 0);

		FRotator RotY = FRotator(0, 0, FMath::Fmod(Offset.Y + TimeY, 360));

		FRotator RotZ = FRotator(FMath::Fmod(Offset.Z + TimeZ, 360), 0, 0);

		FVector A = Rot.RotateVector(FVector(Radius.X, 0, 0)),
			B = RotY.RotateVector(FVector(0, Radius.Y, 0)),
			C = RotZ.RotateVector(FVector(0, 0, Radius.Z));

		ComponentToSolve->SetRelativeLocation(OrbitPivot + A + B + C, false, nullptr, ETeleportType::TeleportPhysics);
	}
};
