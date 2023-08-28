#pragma once

#include "EngineMinimal.h"

// 灯光常量缓冲区

struct FLightConstantBuffer
{
	FLightConstantBuffer();

	XMFLOAT3 LightIntensity;		// 光照强度
	XMFLOAT3 LightDirection;		// 光照方向
};

