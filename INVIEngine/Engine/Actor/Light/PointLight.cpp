#include "EngineMinimal.h"

#include "PointLight.h"

#include "Component/Light/PointLightComponent.h"

GPointLight::GPointLight()
{
	CLight::SetLightComponent(CreateObject<CPointLightComponent>(new CPointLightComponent()));
}

GPointLight::~GPointLight()
{
}

void GPointLight::BeginInit()
{
	CLight::BeginInit();
}

void GPointLight::Tick(float DeltaTime)
{
	CLight::Tick(DeltaTime);

	time += DeltaTime;

	XMFLOAT3 v3 = GetPosition();

	v3.x -= cos(time) * 0.3f;

	SetPosition(v3);
}


