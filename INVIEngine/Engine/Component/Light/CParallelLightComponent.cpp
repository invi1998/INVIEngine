#include "CParallelLightComponent.h"

CParallelLightComponent::CParallelLightComponent()
{
}

CParallelLightComponent::~CParallelLightComponent()
{
}

void CParallelLightComponent::BeginInit()
{
	CLightComponent::BeginInit();
}

void CParallelLightComponent::Tick(float DeltaTime)
{
	CLightComponent::Tick(DeltaTime);
}
