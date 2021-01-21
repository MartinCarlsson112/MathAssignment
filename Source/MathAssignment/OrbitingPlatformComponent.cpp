#include "OrbitingPlatformComponent.h"

UOrbitingPlatformComponent::UOrbitingPlatformComponent()
{

	PrimaryComponentTick.bCanEverTick = false;
	SetAbsolute(true, true);
}



