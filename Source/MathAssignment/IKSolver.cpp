#include "IKSolver.h"
#include "Kismet/KismetMathLibrary.h"

static FTransform Combine(const FTransform& A, const FTransform& B)
{
	return A + B;
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
	FVector End = WorldTransform.GetRotation().RotateVector(FVector(IKChain.Chain[Index]->IKData.Length, 0, 0));
	return WorldTransform.GetLocation() + End;
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




bool UIKSolver::SolveIKChainCCD(const FIKChain2& IKChain, float Threshold /* = 0.0001f*/, int Steps/*= 15*/)
{
	bool ValidIKChain = IsIKChainValid(IKChain);

	ensure(ValidIKChain);
	if (!ValidIKChain)
	{
		return false;
	}

	int Size = IKChain.Chain.Num();
	int Last = IKChain.Chain.Num() - 1;

	float ThresholdSq = Threshold * Threshold;
	FVector Goal = IKChain.Target;

	for (int i = 0; i < Steps; i++)
	{
		FVector Effector = GetGlobalPosition(IKChain, Last);
		if (FVector::DistSquared(Goal, Effector) < ThresholdSq)
		{
			return true;
		}

		//Solve iterations
		for (int j = Size - 1; j >= 0; --j)
		{
			Effector = GetGlobalPosition(IKChain, Last);

			FTransform World = GetGlobalTransform(IKChain, j);
			FVector Position = World.GetLocation();
			FQuat Rotation = World.GetRotation();

			//find a vector from the position of the joint to the end effector position
			FVector ToEffector = Effector - Position;

			//find another vector from the position of the current joint to the position of the goal
			FVector ToGoal = Goal - Position;

			FQuat EffectorToGoal;
			if (FVector::DistSquared(Goal, Position) > ThresholdSq)
			{		
				//find a quat that rotates from the position to effector vector to the position of goal 
				EffectorToGoal = FQuat::FindBetween(ToEffector, ToGoal);

				//rotate the world space orientation of the joint by the inverse of the joint's previous world rotation to move the quaternion back into the joint space
				FQuat WorldRotated = Rotation * EffectorToGoal;
				FQuat LocalRotate = WorldRotated * Rotation.Inverse();

				IKChain.Chain[j]->SetRelativeRotation(LocalRotate * Rotation);
				//as the joint moves, check how close the end effector moved to the goal at each iteration

				Effector = GetGlobalPosition(IKChain, Last);
				//if close enough return true
				if (FVector::DistSquared(Effector, Goal) < ThresholdSq)
				{
					return true;
				}
			}
		}
		return false;
	}

	return false;
}

void UIKSolver::SolveIKChainMultiCCD(const TArray<FIKChain2>& IKChain, float Threshold /*= 0.0001f*/, int Steps /*= 15*/)
{
	for (int i = 0; i < IKChain.Num(); i++)
	{
		SolveIKChainCCD(IKChain[i], Threshold, Steps);
	}
}

void UIKSolver::SolveIKChainFABRIK(const FIKChain2& IKChain)
{
	TArray<FTransform> ChainTransforms;
	FVector StartPosition = IKChain.Start;
	IKChain.Chain[IKChain.Size-1]->SetWorldLocation(IKChain.Target);

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
	SolveFabrikRotations(IKChain);
}

void UIKSolver::SolveFabrikRotations(const FIKChain2& IKChain)
{
	FQuat ToRot = FQuat();
	for (int i = 0; i < IKChain.Size - 1; i++)
	{
		ToRot = LookRotation(IKChain.Chain[i]->GetComponentLocation() - IKChain.Chain[i+1]->GetComponentLocation(), FVector(0, 0, 1));
		IKChain.Chain[i]->SetWorldRotation(ToRot);
	}

	if (FVector::Distance(IKChain.Chain[IKChain.Size - 1]->GetComponentLocation(), IKChain.Target) > IKChain.Chain[IKChain.Size - 1]->IKData.Length)
	{
		ToRot = LookRotation(IKChain.Chain[IKChain.Size - 1]->GetComponentLocation() - IKChain.Target, FVector(0, 0, 1));
		IKChain.Chain[IKChain.Size - 1]->SetWorldRotation(ToRot);
	}
	else
	{
		IKChain.Chain[IKChain.Size - 1]->SetWorldRotation(ToRot);
	}
}

FIKChain2 UIKSolver::CreateIKChain(TArray<UIKComponent*> Bones, const FVector& Start, const FVector& Target)
{
	FIKChain2 Chain;

	const int Size = Bones.Num();

	for (int i = 0; i < Size; i++)
	{
		Chain.Chain.Add(Bones[i]);
	}
	Chain.Start = Start;
	Chain.Target = Target;
	Chain.Size = Size;
	Chain.TotalLength = 0;
	return Chain;
}
