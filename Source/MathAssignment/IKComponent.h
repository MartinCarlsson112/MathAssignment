#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IKBone.h"
#include "IKComponent.Generated.h"


UCLASS( ClassGroup=(IK), meta=(BlueprintSpawnableComponent) )
class MATHASSIGNMENT_API UIKComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:	
	UIKComponent() {
		PrimaryComponentTick.bCanEverTick = false;
	}

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = IK)
	FIKBone IKData;
};
