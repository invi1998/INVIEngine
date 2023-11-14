#include "EngineMinimal.h"
#include "FogComponent.h"

CFogComponent::CFogComponent(): FogColor(Colors::White), FogStart(5.f), FogRange(100.f), FogHeight(100.f), bDirty(false)
{
}

void CFogComponent::SetFogColor(const XMFLOAT4& color)
{
	FogColor = color;
	SetDirty(true);
}

void CFogComponent::SetFogStart(float start)
{
	FogStart = start;
	SetDirty(true);
}

void CFogComponent::SetFogRange(float range)
{
	FogRange = range;
	SetDirty(true);
}

void CFogComponent::SetFogHeight(float height)
{
	FogHeight = height;
	SetDirty(true);
}

void CFogComponent::SetDirty(bool dirty)
{
	bDirty = dirty;
}
