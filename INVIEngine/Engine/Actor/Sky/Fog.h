#pragma once
#include "Actor/Core/ActorObject.h"

class CFogComponent;

class GFog : public GActorObject
{
	CVARIABLE()
		CFogComponent* FogComponent;

public:
	GFog();

	void SetFogColor(const XMFLOAT4& color);
	void SetFogStart(float start);
	void SetFogRange(float range);

};

