#pragma once

#include "EngineMinimal.h"

// �ƹⳣ��������

struct FLightConstantBuffer
{
	FLightConstantBuffer();

	XMFLOAT3 LightIntensity;		// ����ǿ��
	float xx = 0.f;
	XMFLOAT3 LightDirection;		// ���շ���
	float xx1 = 0.f;
};

