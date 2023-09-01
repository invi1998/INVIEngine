#pragma once

#include "EngineMinimal.h"

struct FViewportInfo
{
	// ���������λ�ã�up, right, look �ӿھ���
	XMFLOAT4X4 ViewMatrix;

	// ���ӷ�Χ ���ӽǶ� ͶӰ����
	XMFLOAT4X4 ProjectionMatrix;

	XMFLOAT4 CameraPosition;
};

