#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
	ParallelLightComponent = CreateObject<CParallelLightComponent>(new CParallelLightComponent());
}

GParallelLight::~GParallelLight()
{
}

void GParallelLight::BeginInit()
{
	CCoreMinimalObject::BeginInit();
}

void GParallelLight::Tick(float DeltaTime)
{
	CCoreMinimalObject::Tick(DeltaTime);
}

void GParallelLight::SetPosition(const XMFLOAT3& position)
{
	GActorObject::SetPosition(position);

	ParallelLightComponent->SetPosition(position);
}

void GParallelLight::SetRotation(const fvector_3d& rotation)
{
	GActorObject::SetRotation(rotation);

	ParallelLightComponent->SetRotation(rotation);
}

void GParallelLight::SetScale(const fvector_3d& scale)
{
	GActorObject::SetScale(scale);
	ParallelLightComponent->SetScale(scale);
}

