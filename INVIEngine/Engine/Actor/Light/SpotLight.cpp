#include "SpotLight.h"

#include "Component/Light/SpotLightComponent.h"

GSpotLight::GSpotLight()
{
	SpotLightComponent = CreateObject<CSpotLightComponent>(new CSpotLightComponent());
}

GSpotLight::~GSpotLight()
{
}

void GSpotLight::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void GSpotLight::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);

	//XMFLOAT3 v3 = GetRotation();

	//v3.x += DeltaTime * 40.f;
	//v3.y += DeltaTime * 40.f;
	////v3.z += DeltaTime * 100.f;

	//SetRotation(fvector_3d(v3.x, v3.y, v3.z));

}

void GSpotLight::SetPosition(const XMFLOAT3& position)
{
	GActorObject::SetPosition(position);

	SpotLightComponent->SetPosition(position);
}

void GSpotLight::SetRotation(const fvector_3d& rotation)
{
	GActorObject::SetRotation(rotation);

	SpotLightComponent->SetRotation(rotation);
}

void GSpotLight::SetScale(const fvector_3d& scale)
{
	GActorObject::SetScale(scale);
	SpotLightComponent->SetScale(scale);
}

