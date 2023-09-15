#pragma once

#include "EngineMinimal.h"

struct FLight
{
	XMFLOAT3 LightIntensity;		// ����ǿ��
	float xx = 0.f;
	XMFLOAT3 LightDirection;		// ���շ���
	float xx1 = 0.f;
};

// �ƹⳣ��������
struct FLightConstantBuffer
{
	FLightConstantBuffer();

	FLight SceneLights[16];
};

