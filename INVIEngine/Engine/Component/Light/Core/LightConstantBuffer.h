#pragma once

#include "EngineMinimal.h"

// �ƹⳣ��������

struct FLightConstantBuffer
{
	FLightConstantBuffer();

	XMFLOAT3 LightIntensity;		// ����ǿ��
	XMFLOAT3 LightDirection;		// ���շ���
};

