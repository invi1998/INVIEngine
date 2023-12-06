#include "EngineMinimal.h"

#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
	CLight::SetLightComponent(CreateObject<CParallelLightComponent>(new CParallelLightComponent()));
}

GParallelLight::~GParallelLight()
{
}

void GParallelLight::BeginInit()
{
	CLight::BeginInit();
}

void GParallelLight::Tick(float DeltaTime)
{
	CLight::Tick(DeltaTime);

	XMFLOAT3 v3 = GetRotation();

	// v3.x += DeltaTime * 40.f;
	v3.y += DeltaTime * 40.f;
	//v3.z += DeltaTime * 100.f;

	SetRotation(fvector_3d(v3.x, v3.y, v3.z));
	
}


