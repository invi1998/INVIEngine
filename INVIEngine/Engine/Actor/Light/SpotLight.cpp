#include "EngineMinimal.h"

#include "SpotLight.h"

#include "Component/Light/SpotLightComponent.h"

static float index_test = 0.f;

GSpotLight::GSpotLight()
{
	CLight::SetLightComponent(CreateObject<CSpotLightComponent>(new CSpotLightComponent()));
	index_test = 0.f;
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

	index_test += DeltaTime;

	float O = 50.f;
	float I = 30.f;

	SetConicalInnerCorner(I * fabsf(cos(index_test)) + 10.f);
	SetConicalOuterCorner(O * fabsf(cos(index_test)) + 20.f);

	XMFLOAT3 v3 = GetRotation();
	v3.x += DeltaTime * 40.f;
	v3.y += DeltaTime * 40.f;
	////v3.z += DeltaTime * 100.f;

	SetRotation(fvector_3d(v3.x, v3.y, v3.z));
}

float GSpotLight::GetConicalInnerCorner() const
{
	if (const auto SpotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return SpotLightComponent->GetSpotInnerCornerPhi();
	}
	return 0.f;
}

float GSpotLight::GetConicalOuterCorner() const
{
	if (const auto SpotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return SpotLightComponent->GetSpotOuterCornerTheta();
	}
	return 0.f;
}

void GSpotLight::SetConicalInnerCorner(float InConicalInnerCorner)
{
	if (const auto SpotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return SpotLightComponent->SetSpotInnerCornerPhi(InConicalInnerCorner);
	}
}

void GSpotLight::SetConicalOuterCorner(float InConicalOuterCorner)
{
	if (const auto SpotLightComponent = dynamic_cast<CSpotLightComponent*>(GetLightComponent()))
	{
		return SpotLightComponent->SetSpotOuterCornerTheta(InConicalOuterCorner);
	}
}

