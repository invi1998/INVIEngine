#pragma once

#include "EngineMinimal.h"

// 灯光常量缓冲区

struct FLightConstantBuffer
{
	FLightConstantBuffer();

	XMFLOAT3 LightIntensity;		// 光照强度
	float xx = 0.f;
	XMFLOAT3 LightDirection;		// 光照方向
	float xx1 = 0.f;
};

