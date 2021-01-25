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
	static bool SolveIKChainFABRIK(const FIKChain2& IKChain);


	UFUNCTION(BlueprintCallable, Category = IK)
	static FIKChain2 CreateIKChain(TArray<UIKComponent*> Bones, const FVector& Start, const FVector& Target);

};
