#include "IKSolver.h"

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
		for (int j = Size - 2; j >= 0; --j)
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
					EffectorToGoal = FQuat::FindBetween(Effector, ToGoal);
					Rotation = GetGlobalTransform(IKChain, Last).GetRotation();
					WorldRotated = Rotation * EffectorToGoal;
					LocalRotate = WorldRotated * Rotation.Inverse();
					IKChain.Chain[Last]->SetRelativeRotation((LocalRotate * Rotation));
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

bool UIKSolver::SolveIKChainFABRIK(const FIKChain2& IKChain)
{
	return false;
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
