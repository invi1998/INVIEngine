#include "EngineMinimal.h"
#include "Fog.h"

#include "Component/Sky/FogComponent.h"

GFog::GFog()
{
	FogComponent = CreateObject<CFogComponent>(new CFogComponent());
}

void GFog::SetFogColor(const XMFLOAT4& color)
{
	FogComponent->SetFogColor(color);
}

void GFog::SetFogStart(float start)
{
	FogComponent->SetFogStart(start);
}

void GFog::SetFogRange(float range)
{
	FogComponent->SetFogRange(range);
}
