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

void GPointLight::SetStartAttenuation(float Start)
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetStartAttenuation(Start);
	}
}

void GPointLight::SetEndAttenuation(float end)
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetEndAttenuation(end);
	}
}

void GPointLight::SetKc(float c)
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetKc(c);
	}
}

void GPointLight::SetKl(float l)
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetKl(l);
	}
}

void GPointLight::SetKq(float q)
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		spotLightComponent->SetKq(q);
	}
}

float GPointLight::GetStartAttenuation() const
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetStartAttenuation();
	}

	return 0.f;
}

float GPointLight::GetEndAttenuation() const
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetEndAttenuation();
	}

	return 0.f;
}

float GPointLight::GetKc() const
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetKc();
	}

	return 0.f;
}

float GPointLight::GetKl() const
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetKl();
	}

	return 0.f;
}

float GPointLight::GetKq() const
{
	if (CPointLightComponent* spotLightComponent = dynamic_cast<CPointLightComponent*>(GetLightComponent()))
	{
		return spotLightComponent->GetKq();
	}

	return 0.f;
}


