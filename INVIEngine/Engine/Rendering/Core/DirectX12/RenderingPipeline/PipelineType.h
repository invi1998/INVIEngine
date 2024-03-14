#pragma once

#include "EngineMinimal.h"

// ��Ⱦ����״̬���� ö�٣�PSO��
enum EPipelineState : UINT
{
	Transparent = 0,				// ͸��
	AlphaTest = 1,					// Alpha����
	Background = 2,					// ������
	Reflector = 3,					// �����
	Solid = 4,						// ʵ��
	Wireframe = 5,					// �߿�
	OrthographicShadow = 6,			// ������Ӱ
	PerspectiveShadow = 7,			// ͸����Ӱ
	ViewtianeShadow = 8,			// ȫ����Ӱ
	Selection = 9,					// ѡ��
	Operation = 10,					// �����ֱ�
	RotatePlane = 11,				// ��תƽ��


	ScreenNormal = 20,				// ��Ļ����
	SSA0 = 21,						// SSAO
};


// ��Ⱦ����ö������
enum ERenderCondition : UINT
{
	RC_Always = 0,		// ���ǻ���
	RC_Shadow = 1,		// ������Ӱ��Ҫ�����ж�
};
