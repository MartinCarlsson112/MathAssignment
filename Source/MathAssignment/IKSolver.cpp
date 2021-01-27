#include "IKSolver.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
static FTransform Combine(const FTransform& A, const FTransform& B)
{
	return A + B;
}

static FVector GetBoneEnd(const UIKComponent* Component)
{
	return Component->GetComponentLocation() - Component->GetUpVector() * Component->IKData.Length;
}

static FQuat FromTo(const FVector& From, const FVector& To)
{
	FVector F = From.GetSafeNormal(), T = To.GetSafeNormal();
	
	if (F.Equals(T))
	{
		return FQuat();	
	}
	else if (F.Equals(T * -1.0f))
	{
		FVector Orthographic = FVector(1, 0, 0);
		if (FMath::Abs(F.Y) < FMath::Abs(F.X))
		{
			Orthographic = FVector(0, 1, 0);
		}

		if (FMath::Abs(F.Y) < FMath::Abs(F.Z) && FMath::Abs(F.Y) < FMath::Abs(F.X))
		{
			Orthographic = FVector(0, 0, 1);
		}
		FVector Axis = FVector::CrossProduct(F, Orthographic).GetSafeNormal();
		return FQuat(Axis.X, Axis.Y, Axis.Z, 0);
	}

	FVector Half = (F + T).GetSafeNormal();
	FVector Axis = FVector::CrossProduct(F, Half);
	return FQuat(Axis.X, Axis.Y, Axis.Z, FVector::DotProduct(F, Half));
}



static FTransform GetGlobalTransform(const FIKChain2& IKChain, int Index)
{
	//int size = IKChain.Size;
	//FTransform World = IKChain.Chain[Index]->GetRelativeTransform();

	//for (int i = Index- 1; i >= 0; --i) 
	//{
	//	World = Combine(IKChain.Chain[i]->GetRelativeTransform(), World);
	//}
	//return World;

	return IKChain.Chain[Index]->GetComponentTransform();
}

static FVector GetGlobalPosition(const FIKChain2& IKChain, int Index)
{
	FTransform WorldTransform = GetGlobalTransform(IKChain, Index);
	return WorldTransform.GetLocation();
}


FQuat LookRotation(FVector lookAt, FVector upDirection)
{
	FVector forward = lookAt;
	FVector up = upDirection;


	forward = forward.GetSafeNormal();
	up = up - (forward * FVector::DotProduct(up, forward));
	up = up.GetSafeNormal();


	FVector vector = forward.GetSafeNormal();
	FVector vector2 = FVector::CrossProduct(up, vector);
	FVector vector3 = FVector::CrossProduct(vector, vector2);
	float m00 = vector2.X;
	float m01 = vector2.Y;
	float m02 = vector2.Z;
	float m10 = vector3.X;
	float m11 = vector3.Y;
	float m12 = vector3.Z;
	float m20 = vector.X;
	float m21 = vector.Y;
	float m22 = vector.Z;


	float num8 = (m00 + m11) + m22;
	FQuat quaternion = FQuat();
	if (num8 > 0.0f)
	{
		float num = (float)FMath::Sqrt(num8 + 1.0f);
		quaternion.W = num * 0.5f;
		num = 0.5f / num;
		quaternion.X = (m12 - m21) * num;
		quaternion.Y = (m20 - m02) * num;
		quaternion.Z = (m01 - m10) * num;
		quaternion;

		return quaternion;
	}
	if ((m00 >= m11) && (m00 >= m22))
	{
		float num7 = (float)FMath::Sqrt(((1.0f + m00) - m11) - m22);
		float num4 = 0.5f / num7;
		quaternion.X = 0.5f * num7;
		quaternion.Y = (m01 + m10) * num4;
		quaternion.Z = (m02 + m20) * num4;
		quaternion.W = (m12 - m21) * num4;
		quaternion;

		return quaternion;
	}
	if (m11 > m22)
	{
		float num6 = (float)FMath::Sqrt(((1.0f + m11) - m00) - m22);
		float num3 = 0.5f / num6;
		quaternion.X = (m10 + m01) * num3;
		quaternion.Y = 0.5f * num6;
		quaternion.Z = (m21 + m12) * num3;
		quaternion.W = (m20 - m02) * num3;
		quaternion;

		return quaternion;
	}
	float num5 = (float)FMath::Sqrt(((1.0f + m22) - m00) - m11);
	float num2 = 0.5f / num5;
	quaternion.X = (m20 + m02) * num2;
	quaternion.Y = (m21 + m12) * num2;
	quaternion.Z = 0.5f * num5;
	quaternion.W = (m01 - m10) * num2;
	quaternion;

	return quaternion;
}




bool UIKSolver::SolveIKChainCCD(UObject* WorldContextObject, const FIKChain2& IKChain, USceneComponent* TargetPoint, float Threshold /* = 0.0001f*/, int Steps/*= 15*/, bool bDebug /*= false*/)
{
	bool ValidIKChain = IsIKChainValid(IKChain);
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return false;
	}
	ensure(ValidIKChain);
	if (!ValidIKChain)
	{
		return false;
	}

	int Size = IKChain.Chain.Num();
	int Last = IKChain.Chain.Num() - 1;
	float ThresholdSquare = Threshold * Threshold;
	FVector TargetPosition = TargetPoint->GetComponentLocation();
	//DrawDebugSphere(World, TargetPosition, 30, 16, FColor::Red, true, 1.0f);

	for (int i = 0; i < Steps; i++)
	{
		FVector EndEffector = GetBoneEnd(IKChain.Chain[Last]);
		///DrawDebugSphere(World, EndEffector, 30, 16, FColor::Yellow, true, 1.0f);

		if (FVector::DistSquared(TargetPosition, EndEffector) < ThresholdSquare)
		{
			//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Cyan, FString("Already Solved!"));
			//already solved
			return true;
		}

		for(int j = Size - 1; j >= 0; j--)
		{
			EndEffector = GetBoneEnd(IKChain.Chain[Last]);

			FVector WorldPosition = IKChain.Chain[j]->GetComponentLocation();
			FQuat WorldRotation = IKChain.Chain[j]->GetComponentRotation().Quaternion();


			FVector ToEffector = EndEffector - WorldPosition;
			//DrawDebugLine(World, WorldPosition, WorldPosition + ToEffector.GetSafeNormal()* 100.0f, FColor::Yellow, true, 1.0f, 10);

			FVector ToTarget = TargetPosition - WorldPosition;
			//DrawDebugLine(World, WorldPosition, WorldPosition + ToTarget.GetSafeNormal() * 100.0f, FColor::Red, true, 1.0f, 10);

			if (ToTarget.SizeSquared() > ThresholdSquare)
			{
				FQuat EffectorToTarget = FromTo(ToEffector, ToTarget);

				//DrawDebugLine(World, WorldPosition, WorldPosition +  EffectorToGoal.Vector() * 100.0f, FColor::Blue, true, 1.0f, 10);

				FQuat WorldRotated = WorldRotation * EffectorToTarget;
				FQuat LocalRotated = WorldRotated * WorldRotation.Inverse();


				IKChain.Chain[j]->SetWorldRotation(LocalRotated * WorldRotation);


				EndEffector = GetBoneEnd(IKChain.Chain[Last]);
				if (FVector::DistSquared(TargetPosition, EndEffector) < ThresholdSquare)
				{	
					//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Cyan, FString("Success!"));
					return true;
				}
			}
		}
	}
	//GEngine->AddOnScreenDebugMessage(INDEX_NONE, 1.0f, FColor::Cyan, FString("Fail!"));
	return false;
}

void UIKSolver::SolveIKChainMultiCCD(UObject* WorldContextObject, const TArray<FIKChain2>& IKChain, USceneComponent* TargetPoint, float Threshold /*= 0.0001f*/, int Steps /*= 15*/, bool bDebug /*= false*/)
{
	for (int i = 0; i < IKChain.Num(); i++)
	{
		SolveIKChainCCD(WorldContextObject, IKChain[i], TargetPoint, Threshold, Steps, bDebug);
	}
}

void UIKSolver::SolveIKChainFABRIK(const FIKChain2& IKChain, USceneComponent* TargetPoint)
{
	bool ValidIKChain = IsIKChainValid(IKChain);
	ensure(ValidIKChain);
	if (!ValidIKChain)
	{
		return;
	}
	TArray<FTransform> ChainTransforms;
	FVector StartPosition = IKChain.Start->GetComponentLocation();
	IKChain.Chain[IKChain.Size-1]->SetWorldLocation(TargetPoint->GetComponentLocation());

	for (int i = IKChain.Size - 2; i >= 0 ; --i)
	{
		FVector Current = IKChain.Chain[i]->GetComponentLocation();
		FVector Next = IKChain.Chain[i +1]->GetComponentLocation();
		FVector Direction = Current - Next;

		Direction.Normalize();

		FVector Offset = Direction * IKChain.Chain[i + 1]->IKData.Length;

		IKChain.Chain[i]->SetWorldLocation(Next + Offset);
	}

	IKChain.Chain[0]->SetWorldLocation(StartPosition);

	for (int i = 1; i < IKChain.Size; ++i)
	{
		FVector Current = IKChain.Chain[i]->GetComponentLocation();
		FVector Prev = IKChain.Chain[i-1]->GetComponentLocation();
		FVector Direction = Current - Prev;
		Direction.Normalize();

		FVector Offset = Direction * IKChain.Chain[i]->IKData.Length;

		IKChain.Chain[i]->SetWorldLocation(Prev + Offset);
	}
	SolveFabrikRotations(IKChain, TargetPoint);
}

void UIKSolver::SolveFabrikRotations(const FIKChain2& IKChain, USceneComponent* TargetPoint)
{
	FQuat ToRot = FQuat();
	for (int i = 0; i < IKChain.Size - 1; i++)
	{
		ToRot = LookRotation(IKChain.Chain[i]->GetComponentLocation() - IKChain.Chain[i+1]->GetComponentLocation(), FVector(0, 0, 1));
		IKChain.Chain[i]->SetWorldRotation(ToRot);
	}

	FVector TargetPos = TargetPoint->GetComponentLocation();

	if (FVector::Distance(IKChain.Chain[IKChain.Size - 1]->GetComponentLocation(), TargetPos) > IKChain.Chain[IKChain.Size - 1]->IKData.Length)
	{
		ToRot = LookRotation(IKChain.Chain[IKChain.Size - 1]->GetComponentLocation() - TargetPos, FVector(0, 0, 1));
		IKChain.Chain[IKChain.Size - 1]->SetWorldRotation(ToRot);
	}
	else
	{
		IKChain.Chain[IKChain.Size - 1]->SetWorldRotation(ToRot);
	}
}

bool UIKSolver::StepSolver(UObject* WorldContextObject, USceneComponent* TargetPoint, USceneComponent* PolePoint, const FVector BasePosition, float Threshold, AActor* IgnoredActor, FVector& NewPosition, bool bDebug)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::ReturnNull);
	if (!World)
	{
		return false;
	}
	FVector PolePosition = PolePoint->GetComponentLocation();
	float Dist = FVector::Distance(TargetPoint->GetComponentLocation(), PolePosition);
	if (Dist > Threshold)
	{
		FHitResult HitResult;
		if (bDebug)
		{
			DrawDebugLine(World, BasePosition, PolePosition, FColor::Red, true, 0.5f);

		}


		FCollisionQueryParams Params;
		Params.AddIgnoredActor(IgnoredActor);
		if (World->LineTraceSingleByChannel(HitResult, BasePosition, PolePosition, ECC_Visibility, Params))
		{
			if (bDebug)
			{
				DrawDebugSphere(World, HitResult.Location, 30, 26, FColor(181, 0, 0), false, 0.5f, 0, 2);
				//draw hit point
			}

			NewPosition = HitResult.Location;
			return true;
		}
	}
	return false;
}


FIKChain2 UIKSolver::CreateIKChain(TArray<UIKComponent*> Bones, USceneComponent* Start)
{
	FIKChain2 Chain;

	const int Size = Bones.Num();

	for (int i = 0; i < Size; i++)
	{
		Chain.Chain.Add(Bones[i]);
	}
	Chain.Start = Start;
	Chain.Size = Size;
	Chain.TotalLength = 0;
	return Chain;
}
