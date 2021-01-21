#include "OrbitingPlatform.h"


AOrbitingPlatform::AOrbitingPlatform()
{
 	PrimaryActorTick.bCanEverTick = false;
}


FRotator CombineRotators(FRotator A, FRotator B)
{
	FQuat AQuat = FQuat(A);
	FQuat BQuat = FQuat(B);

	return FRotator(BQuat * AQuat);
}

void AOrbitingPlatform::OrbitSolveMultiple(TArray<UOrbitingPlatformComponent*> ComponentToSolve, FVector OrbitPivot, bool bUseVInterp, float VInterpSpeed/* = 1*/)
{
	int nSolves = ComponentToSolve.Num();
	for (int i = 0; i < nSolves; i++)
	{
		if (ComponentToSolve[i] == nullptr)
		{
			continue;
		}
		float TimeInSeconds = GetWorld()->GetTimeSeconds();

	
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

		if (bUseVInterp)
		{
			FVector TargetPosition = FMath::VInterpTo(ComponentToSolve[i]->GetRelativeLocation(), OrbitPivot + A + B + C, GetWorld()->GetDeltaSeconds(), VInterpSpeed);
			ComponentToSolve[i]->SetRelativeLocation(TargetPosition, false, nullptr, ETeleportType::TeleportPhysics);
		}
		else
		{
			ComponentToSolve[i]->SetRelativeLocation(OrbitPivot + A + B + C, false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}

void AOrbitingPlatform::OrbitSolve(UOrbitingPlatformComponent* ComponentToSolve, FVector OrbitPivot /*= FVector(0, 0, 0)*/, bool bUseVInterp /*= true*/, float VInterpSpeed /*= 1*/)
{
	if (ComponentToSolve == nullptr)
	{
		return;
	}
	float TimeInSeconds = GetWorld()->GetTimeSeconds();


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

	if (bUseVInterp)
	{
		FVector TargetPosition = FMath::VInterpTo(ComponentToSolve->GetRelativeLocation(), OrbitPivot + A + B + C, GetWorld()->GetDeltaSeconds(), VInterpSpeed);
		ComponentToSolve->SetRelativeLocation(TargetPosition, false, nullptr, ETeleportType::TeleportPhysics);
	}
	else
	{
		ComponentToSolve->SetRelativeLocation(OrbitPivot + A + B + C, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

