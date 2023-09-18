#pragma once

#include "EngineMinimal.h"

struct FLight
{
    FLight();

    XMFLOAT3 LightIntensity;  // 光照强度
    float StartAttenuation; // 开始衰减

    XMFLOAT3 LightDirection;  // 光照方向
    float EndAttenuation;   // 结束衰减

    XMFLOAT3 LightPosition;    // 灯光位置
    int LightType;          // 灯光类型
};

// 灯光常量缓冲区
struct FLightConstantBuffer
{
	FLightConstantBuffer();

	FLight SceneLights[16];
};

