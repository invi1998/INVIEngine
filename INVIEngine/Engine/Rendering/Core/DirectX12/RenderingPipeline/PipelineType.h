#pragma once

#include "EngineMinimal.h"

// ��Ⱦ����״̬���� ö��

enum EPipelineState : UINT
{
	Transparent = 0,
	AlphaTest,
	Background,					// ������
	Solid	= 4,				// ʵ��
	Wireframe	= 5,			// �߿�
};

