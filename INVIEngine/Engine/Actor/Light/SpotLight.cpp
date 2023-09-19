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

void GSpotLight::SetKc(float c)
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetKc(c);
	}
}

void GSpotLight::SetKl(float l)
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetKl(l);
	}
}

void GSpotLight::SetKq(float q)
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetKq(q);
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

float GSpotLight::GetKc() const
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetKc();
	}

	return 0.f;
}

float GSpotLight::GetKl() const
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetKl();
	}

	return 0.f;
}

float GSpotLight::GetKq() const
{
	if (CSpotLightComponent* spotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetKq();
	}

	return 0.f;
}


