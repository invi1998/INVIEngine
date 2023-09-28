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

	//XMFLOAT3 v3 = GetRotation();

	//v3.x += DeltaTime * 40.f;
	//v3.y += DeltaTime * 40.f;
	////v3.z += DeltaTime * 100.f;

	//SetRotation(fvector_3d(v3.x, v3.y, v3.z));

}


