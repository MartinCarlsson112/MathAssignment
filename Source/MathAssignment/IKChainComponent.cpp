#include "IKChainComponent.h"

#include "IKSolver.h"

UIKChainComponent::UIKChainComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	NumberOfSegments = 4;

	LimbLength = 100;
}
 
void UIKChainComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UIKChainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UIKChainComponent::OnConstruction(const FTransform& Transform)
{
	for (int i = 0; i < NumberOfSegments; i++)
	{
		FName SegmentName = *("Segment" + FGuid::NewGuid().ToString());
		auto IKComponent = NewObject<UIKComponent>(this, SegmentName);
		IKComponent->SetupAttachment(this);
		if (i + 1 == NumberOfSegments)
		{

		}
		else
		{
			IKComponent->SetStaticMesh(MeshToUse);
		}
		
		IKComponent->RegisterComponent();
		IKComponent->IKData.Length = LimbLength;
		IKComponents.Add(IKComponent);
	}
	IkChain = UIKSolver::CreateIKChain(IKComponents, this);
}

