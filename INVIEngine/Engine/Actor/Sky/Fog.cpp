#include "EngineMinimal.h"
#include "Fog.h"

#include "Component/Sky/FogComponent.h"

GFog::GFog()
{
	FogComponent = CreateObject<CFogComponent>(new CFogComponent());
}

void GFog::SetFogColor(const XMFLOAT4& color) const
{
	FogComponent->SetFogColor(color);
}

void GFog::SetFogStart(float start) const
{
	FogComponent->SetFogStart(start);
}

void GFog::SetFogRange(float range) const
{
	FogComponent->SetFogRange(range);
}

void GFog::SetFogHeight(float height) const
{
	FogComponent->SetFogHeight(height);
}
