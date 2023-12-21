#include "EngineMinimal.h"

#include "Light.h"

#include "Component/Light/Core/LightComponent.h"

CLight::CLight()
	: GActorObject()
	, LightComponent(nullptr)
{
}

void CLight::BeginInit()
{
	GActorObject::BeginInit();
}

CLight::~CLight()
{
}

void CLight::Tick(float DeltaTime)
{
	GActorObject::Tick(DeltaTime);
}

void CLight::SetPosition(const XMFLOAT3& InNewPosition)
{
	GActorObject::SetPosition(InNewPosition);

	if (LightComponent)
	{
		LightComponent->SetPosition(InNewPosition);
	}
}

void CLight::SetRotation(const fvector_3d& InRotation)
{
	GActorObject::SetRotation(InRotation);

	if (LightComponent)
	{
		LightComponent->SetRotation(InRotation);
	}
}

void CLight::SetScale(const fvector_3d& InNewScale)
{
	GActorObject::SetScale(InNewScale);

	if (LightComponent)
	{
		LightComponent->SetScale(InNewScale);
	}
}

void CLight::SetCastShadow(bool shadow) const
{
	if (LightComponent)
	{
		LightComponent->SetCastShadow(shadow);
	}
}

void CLight::SetLightComponent(CLightComponent* newComponent)
{
	LightComponent = newComponent;
}

void CLight::SetLightIntensity(const XMFLOAT3& Intensity)
{
	if (LightComponent)
	{
		LightComponent->SetLightIntensity(Intensity);
	}
}

CLightComponent* CLight::GetLightComponent() const
{
	return LightComponent;
}
