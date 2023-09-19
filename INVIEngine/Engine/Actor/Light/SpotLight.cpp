#include "SpotLight.h"

#include "Component/Light/SpotLightComponent.h"

GSpotLight::GSpotLight()
{
	CLight::SetLightComponent(CreateObject<CSpotLightComponent>(new CSpotLightComponent()));
}

GSpotLight::~GSpotLight()
{
}

void GSpotLight::BeginInit()
{
	CLight::BeginInit();
}

void GSpotLight::Tick(float DeltaTime)
{
	CLight::Tick(DeltaTime);

	//XMFLOAT3 v3 = GetRotation();

	//v3.x += DeltaTime * 40.f;
	//v3.y += DeltaTime * 40.f;
	////v3.z += DeltaTime * 100.f;

	//SetRotation(fvector_3d(v3.x, v3.y, v3.z));

}

void GSpotLight::SetStartAttenuation(float Start)
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetStartAttenuation(Start);
	}
}

void GSpotLight::SetEndAttenuation(float end)
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetEndAttenuation(end);
	}
}

float GSpotLight::GetStartAttenuation() const
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetStartAttenuation();
	}

	return 0.f;
}

float GSpotLight::GetEndAttenuation() const
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetEndAttenuation();
	}

	return 0.f;
}


