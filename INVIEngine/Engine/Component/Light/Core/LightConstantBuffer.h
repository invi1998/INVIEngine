#pragma once

#include "EngineMinimal.h"

struct FLight
{
    FLight();

    XMFLOAT3 LightIntensity;  // ����ǿ��
    float StartAttenuation; // ��ʼ˥��

    XMFLOAT3 LightDirection;  // ���շ���
    float EndAttenuation;   // ����˥��

    XMFLOAT3 LightPosition;    // �ƹ�λ��
    int LightType;          // �ƹ�����
};

// �ƹⳣ��������
struct FLightConstantBuffer
{
	FLightConstantBuffer();

	FLight SceneLights[16];
};

