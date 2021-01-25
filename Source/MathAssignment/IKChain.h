#pragma once
#include "CoreMinimal.h"
#include "IKComponent.h"
#include "IKChain.generated.h"

USTRUCT(BlueprintType)
struct MATHASSIGNMENT_API FIKChain2
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IK)
	TArray<UIKComponent*> Chain;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IK)
	int Size;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IK)
	float TotalLength;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IK)
	FVector Start;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IK)
	FVector Target;
};
