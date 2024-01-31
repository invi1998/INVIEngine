#include "EngineMinimal.h"

#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
	FCreateObjectParams params{};
	params.Owner = this;

	CLight::SetLightComponent(CreateObject<CParallelLightComponent>(params, new CParallelLightComponent()));
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
	v3.y -= DeltaTime * 40.f;
	//v3.z += DeltaTime * 100.f;

	SetRotation(v3);
	
}


