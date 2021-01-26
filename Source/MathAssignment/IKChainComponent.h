#pragma once

#include "CoreMinimal.h"
#include "IKChain.h"
#include "Components/SceneComponent.h"
#include "IKComponent.h"
#include "IKChainComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MATHASSIGNMENT_API UIKChainComponent : public USceneComponent
{
	GENERATED_BODY()

public:	

	UIKChainComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = IK)
	void OnConstruction(const FTransform& Transform);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK)
	TArray<UIKComponent*> IKComponents;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = IK)
	int NumberOfSegments;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = IK)
	UStaticMesh* MeshToUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = IK)
	float LimbLength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = IK)
	FIKChain2 IkChain;



		
};
