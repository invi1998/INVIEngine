#include "LightComponent.h"

CLightComponent::CLightComponent()
{
}

CLightComponent::~CLightComponent()
{
}

void CLightComponent::BeginInit()
{
	CTransformationComponent::BeginInit();
}

void CLightComponent::Tick(float DeltaTime)
{
	CTransformationComponent::Tick(DeltaTime);
}
