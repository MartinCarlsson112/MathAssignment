#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IKComponent.h"
#include "IKChain.h"
#include "IKSolver.generated.h"


UCLASS()
class MATHASSIGNMENT_API UIKSolver : public UObject
{
	GENERATED_BODY()
	
public:	

	//Solve IK using CCD Algorithm
	UFUNCTION(BlueprintCallable, Category = IK)
	static bool SolveIKChainCCD(const FIKChain2& IKChain, float Threshold = 0.0001f, int Steps = 15);

	//Solve IK using CCD Algorithm
	UFUNCTION(BlueprintCallable, Category = IK)
	static void SolveIKChainMultiCCD(const TArray<FIKChain2>& IKChain, float Threshold = 0.0001f, int Steps = 15);

	//Solve IK using FABRIK algorithm
	UFUNCTION(BlueprintCallable, Category = IK)
	static void SolveIKChainFABRIK(const FIKChain2& IKChain);

	UFUNCTION()
	static void SolveFabrikRotations(const FIKChain2& IKChain);


	UFUNCTION(BlueprintCallable, Category = IK)
	static FIKChain2 CreateIKChain(TArray<UIKComponent*> Bones, const FVector& Start, const FVector& Target);

	UFUNCTION(BlueprintCallable, Category = IK)
	static bool IsIKChainValid(FIKChain2 Chain)
	{
		for (int i = 0; i < Chain.Chain.Num(); i++)
		{
			if (Chain.Chain[i] == nullptr)
			{
				return false;
			}
		}
		return Chain.Chain.Num() > 0;
	}
};
