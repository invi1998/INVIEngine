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
	Shadow = 6,					// ��Ӱ
};


// ��Ⱦ����ö������
enum ERenderCondition : UINT
{
	RC_Always = 0,		// ���ǻ���
	RC_Shadow = 1,		// ������Ӱ
};
