#include "LightComponent.h"

#include "Light/LightManager.h"

CLightComponent::CLightComponent()
{
	GetLightManger()->AddLight(this);
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
