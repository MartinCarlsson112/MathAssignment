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
	UFUNCTION(BlueprintCallable, Category = IK, meta = (WorldContext = "WorldContextObject"))
	static bool SolveIKChainCCD(UObject* WorldContextObject, const FIKChain2& IKChain, USceneComponent* TargetPoint, float Threshold = 0.0001f, int Steps = 15, bool bDebug = false);

	//Solve IK using CCD Algorithm
	UFUNCTION(BlueprintCallable, Category = IK, meta = (WorldContext = "WorldContextObject"))
	static void SolveIKChainMultiCCD(UObject* WorldContextObject, const TArray<FIKChain2>& IKChain, USceneComponent* TargetPoint, float Threshold = 0.0001f, int Steps = 15, bool bDebug = false);

	//Solve IK using FABRIK algorithm
	UFUNCTION(BlueprintCallable, Category = IK)
	static void SolveIKChainFABRIK(const FIKChain2& IKChain, USceneComponent* TargetPoint);

	UFUNCTION()
	static void SolveFabrikRotations(const FIKChain2& IKChain, USceneComponent* TargetPoint);

	UFUNCTION(BlueprintCallable, Category = IK, meta = (WorldContext = "WorldContextObject"))
	static bool StepSolver(UObject* WorldContextObject, USceneComponent* TargetPoint, USceneComponent* PolePoint, const FVector BasePosition, float Threshold, AActor* IgnoredActor, FVector & NewPosition, bool bDebug);
	
	
	UFUNCTION(BlueprintCallable, Category = IK)
	static FIKChain2 CreateIKChain(TArray<UIKComponent*> Bones, USceneComponent* Start);

	UFUNCTION(BlueprintCallable, Category = IK)
	static bool IsIKChainValid(FIKChain2 Chain)
	{
		if (Chain.Start == nullptr)
		{
			return false;
		}

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
