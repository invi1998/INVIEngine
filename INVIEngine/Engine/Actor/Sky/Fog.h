#pragma once
#include "Actor/Core/ActorObject.h"

class CFogComponent;

class GFog : public GActorObject
{
	CVARIABLE()
		CFogComponent* FogComponent;

public:
	GFog();

	void SetFogColor(const XMFLOAT4& color) const;
	void SetFogStart(float start) const;
	void SetFogRange(float range) const;
	void SetFogHeight(float height) const;

};

