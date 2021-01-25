#pragma once
#include "CoreMinimal.h"
#include "IKBone.generated.h"

USTRUCT(BlueprintType)
struct FIKBone
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IK)
	float Length;
};
