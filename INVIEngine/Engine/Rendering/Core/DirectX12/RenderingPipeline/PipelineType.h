#pragma once

#include "EngineMinimal.h"

// ��Ⱦ����״̬���� ö��

enum EPipelineState : UINT
{
	Transparent = 0,
	AlphaTest = 1,
	Background = 2,					// ������
	Reflector = 3,					// �����
	Solid	= 4,				// ʵ��
	Wireframe	= 5,			// �߿�
};

